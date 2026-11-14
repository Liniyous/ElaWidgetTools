#include "ElaTabWidgetPrivate.h"

#include "ElaTabBar.h"
#include "ElaTabDragData.h"
#include "ElaTabDropIndicator.h"
#include "ElaTabWidget.h"
#include "ElaTheme.h"

#include <QApplication>
#include <QCursor>
#include <QDrag>
#include <QDropEvent>
#include <QFontMetricsF>
#include <QKeyEvent>
#include <QPainter>
#include <QPixmap>
#include <QStackedWidget>
#include <QTimer>
#include <QtMath>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

ElaTabWidgetPrivate::ElaTabWidgetPrivate(QObject* parent)
    : QObject(parent)
{
}

ElaTabBar* ElaTabWidgetPrivate::getTabBar(int index) const
{
    return index >= 0 && index < _pinnedTabBar->count() ? _pinnedTabBar : _tabBar;
}

int ElaTabWidgetPrivate::getBarIndex(int index) const
{
    return getTabBar(index) == _pinnedTabBar ? index : index - _pinnedTabBar->count();
}

void ElaTabWidgetPrivate::updateTabRows()
{
    _pinnedTabBar->setVisible(_pinnedTabBar->count() > 0);
    _tabBar->setVisible(_tabBar->count() > 0 || _pinnedTabBar->count() == 0);
    Q_Q(ElaTabWidget);
    const bool pinned = q->getIsTabPinned(q->currentIndex());
    _pinnedTabBar->setIsActive(pinned);
    _tabBar->setIsActive(!pinned);
}

void ElaTabWidgetPrivate::startTabDrag(int index)
{
    Q_Q(ElaTabWidget);
    auto* page = q->widget(index);
    if (!page)
    {
        return;
    }
    auto* data = new ElaTabDragData();
    data->_source = q;
    data->_page = page;
    const QPointer<ElaTabWidget> source(q);
    const QPointer<QWidget> draggedPage(page);
    data->setData("application/x-ela-tab", {});
    auto* drag = new QDrag(q);
    // 拖放对象必须存活到原生拖放循环退出
    drag->setParent(qApp);
    const auto cancelConnection = connect(q, &QObject::destroyed, drag, []() {
        QDrag::cancel();
    });
    drag->setMimeData(data);
    const auto preview = _getTabDragPixmap(index);
    drag->setPixmap(preview);
    const auto previewSize = preview.size() / preview.devicePixelRatio();
    drag->setHotSpot(QPoint(qMin(previewSize.width() / 2, 100), previewSize.height() / 2));
    _isDragCanceled = false;
    const auto result = drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::MoveAction);
    disconnect(cancelConnection);
    // 原生拖放循环中浮窗和空组可能已经销毁
    if (!source || result != Qt::IgnoreAction)
    {
        return;
    }
    bool isReleased = !QApplication::mouseButtons().testFlag(Qt::LeftButton);
#ifdef Q_OS_WIN
    // OLE 拖放期间 Qt 缓存的鼠标状态不能区分释放和取消
    isReleased = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0;
#endif
    if (!_isDragCanceled && isReleased)
    {
        const auto position = QCursor::pos() - QPoint(100, 16);
        // 原生拖放返回后先处理 Qt 排队的鼠标释放事件
        QTimer::singleShot(0, q, [q, draggedPage, position]() {
            if (draggedPage && q->indexOf(draggedPage) >= 0)
            {
                q->detachTab(q->indexOf(draggedPage), position);
            }
        });
    }
}

bool ElaTabWidgetPrivate::eventFilter(QObject* watched, QEvent* event)
{
    Q_Q(ElaTabWidget);
    if (event->type() == QEvent::KeyPress && static_cast<QKeyEvent*>(event)->key() == Qt::Key_Escape)
    {
        _isDragCanceled = true;
        _showDropIndicator(ElaTabWidgetType::NoArea);
    }
    if (watched == q && event->type() == QEvent::Hide && _dropIndicator)
    {
        _dropIndicator->hideTarget(false);
    }
    if (!q->isVisible() || (event->type() != QEvent::DragEnter && event->type() != QEvent::DragMove && event->type() != QEvent::Drop && event->type() != QEvent::DragLeave))
    {
        return QObject::eventFilter(watched, event);
    }
    auto* widget = qobject_cast<QWidget*>(watched);
    if (!widget)
    {
        return false;
    }
    const bool isTabBar = widget == _tabBar || widget == _pinnedTabBar || _tabBar->isAncestorOf(widget) || _pinnedTabBar->isAncestorOf(widget);
    if (!isTabBar && widget != q && !q->isAncestorOf(widget))
    {
        return false;
    }
    if (event->type() == QEvent::DragLeave)
    {
        _showDropIndicator(ElaTabWidgetType::NoArea);
        return false;
    }
    auto* drop = static_cast<QDropEvent*>(event);
    auto* source = ElaTabWidget::getDragSource(drop->mimeData());
    auto* page = ElaTabWidget::getDragPage(drop->mimeData());
    if (!source || !page || source->getDragScope() != q->getDragScope() || (!isTabBar && !_pIsContainerAcceptDrops))
    {
        return false;
    }
    auto area = ElaTabWidgetType::Center;
    if (!isTabBar && _isSplitDropEnabled)
    {
        const auto position = widget->mapTo(q, drop->pos());
        const auto bounds = _getContentRect();
        const qreal horizontalEdge = qMax(1, qMin(90, bounds.width() / 4));
        const qreal verticalEdge = qMax(1, qMin(90, bounds.height() / 4));
        qreal nearest = 1;
        for (const auto& candidate: {qMakePair(ElaTabWidgetType::Left, (position.x() - bounds.left()) / horizontalEdge),
                                     qMakePair(ElaTabWidgetType::Right, (bounds.right() - position.x()) / horizontalEdge),
                                     qMakePair(ElaTabWidgetType::Top, (position.y() - bounds.top()) / verticalEdge),
                                     qMakePair(ElaTabWidgetType::Bottom, (bounds.bottom() - position.y()) / verticalEdge)})
        {
            if (candidate.second < nearest)
            {
                nearest = candidate.second;
                area = candidate.first;
            }
        }
    }
    if (event->type() == QEvent::Drop)
    {
        _showDropIndicator(ElaTabWidgetType::NoArea);
        if (!isTabBar)
        {
            const bool pinned = source->getIsTabPinned(source->indexOf(page));
            if (area != ElaTabWidgetType::Center)
            {
                Q_EMIT q->tabTransferRequested(source, page, q->count(), pinned, drop->proposedAction(), area);
            }
            else if (source != q)
            {
                q->transferTab(source, page, q->count(), pinned, drop->proposedAction());
            }
        }
    }
    else
    {
        _showDropIndicator(source != q || area != ElaTabWidgetType::Center ? area : ElaTabWidgetType::NoArea);
    }
    if (!isTabBar)
    {
        drop->acceptProposedAction();
        return true;
    }
    return QObject::eventFilter(watched, event);
}

QPixmap ElaTabWidgetPrivate::_getTabDragPixmap(int index) const
{
    Q_Q(const ElaTabWidget);
    const auto* bar = getTabBar(index);
    const auto icon = q->tabIcon(index);
    const auto iconSize = icon.isNull() ? QSize(0, 0) : bar->iconSize();
    const int iconSpace = icon.isNull() ? 0 : iconSize.width() + 6;
    const QFontMetrics metrics = bar->fontMetrics();
    const auto title = metrics.elidedText(q->tabText(index).simplified(), Qt::ElideRight, 420 - 24 - iconSpace);
    const int titleWidth = qCeil(QFontMetricsF(metrics).horizontalAdvance(title));
    const QSize size(titleWidth + iconSpace + 24, qMax(metrics.height(), iconSize.height()) + 16);
    const qreal ratio = bar->devicePixelRatioF();
    QPixmap preview(qRound(size.width() * ratio), qRound(size.height() * ratio));
    preview.setDevicePixelRatio(ratio);
    preview.fill(Qt::transparent);
    QPainter painter(&preview);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    const auto theme = eTheme->getThemeMode();
    painter.setPen(ElaThemeColor(theme, BasicBorder));
    painter.setBrush(ElaThemeColor(theme, PopupBase));
    painter.drawRoundedRect(QRectF(0.5, 0.5, size.width() - 1, size.height() - 1), 7, 7);
    if (!icon.isNull())
    {
        icon.paint(&painter, QRect(12, (size.height() - iconSize.height()) / 2, iconSize.width(), iconSize.height()));
    }
    painter.setFont(bar->font());
    painter.setPen(ElaThemeColor(theme, BasicText));
    painter.drawText(QRect(12 + iconSpace, 0, size.width() - 24 - iconSpace, size.height()), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, title);
    return preview;
}

QRect ElaTabWidgetPrivate::_getContentRect() const
{
    Q_Q(const ElaTabWidget);
    if (auto* page = q->currentWidget())
    {
        return QRect(page->mapTo(q, QPoint()), page->size());
    }
    return q->rect();
}

void ElaTabWidgetPrivate::_showDropIndicator(ElaTabWidgetType::DropArea area)
{
    if (!_isDropIndicatorEnabled)
    {
        return;
    }
    if (area == ElaTabWidgetType::NoArea)
    {
        _dropIndicator->hideTarget();
        return;
    }
    auto bounds = _getContentRect();
    switch (area)
    {
    case ElaTabWidgetType::Left:
    {
        bounds.setWidth(bounds.width() / 2);
        break;
    }
    case ElaTabWidgetType::Right:
    {
        bounds.setLeft(bounds.left() + bounds.width() / 2);
        break;
    }
    case ElaTabWidgetType::Top:
    {
        bounds.setHeight(bounds.height() / 2);
        break;
    }
    case ElaTabWidgetType::Bottom:
    {
        bounds.setTop(bounds.top() + bounds.height() / 2);
        break;
    }
    default:
    {
        break;
    }
    }
    _dropIndicator->showTarget(bounds);
}
