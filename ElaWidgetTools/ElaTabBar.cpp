#include "ElaTabBar.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

#include "ElaTabBarPrivate.h"
#include "ElaTabBarStyle.h"
#include "private/qtabbar_p.h"
#include <QTimer>
ElaTabBar::ElaTabBar(QWidget* parent)
    : QTabBar(parent), d_ptr(new ElaTabBarPrivate())
{
    Q_D(ElaTabBar);
    d->q_ptr = this;
    setObjectName("ElaTabBar");
    setMouseTracking(true);
    setStyleSheet("#ElaTabBar{background-color:transparent;}");
    setTabsClosable(true);
    setMovable(true);
    setAcceptDrops(true);
    d->_style = new ElaTabBarStyle(style());
    setStyle(d->_style);

    d->_pTargetScrollOffset = 0;
    d->_tabBarPrivate = dynamic_cast<QTabBarPrivate*>(this->QTabBar::d_ptr.data());
    // 关闭自带滚动按钮，避免 layoutTabs 在 tab 溢出时重置 scrollOffset
    setUsesScrollButtons(false);
    connect(d, &ElaTabBarPrivate::pScrollOffsetChanged, this, [=]() {
        // 动画每帧将偏移应用到 QTabBar 官方滚动机制
        d->_tabBarPrivate->scrollOffset = qRound(d->getScrollOffset());
        update();
    });
    // Qt 内部(refresh/makeVisible 等)修改 scrollOffset 时同步动画状态
    connect(this, &QTabBar::currentChanged, this, [=]() {
        d->setTargetScrollOffset(d->_tabBarPrivate->scrollOffset);
        d->setScrollOffset(d->_tabBarPrivate->scrollOffset);
    });
}

ElaTabBar::~ElaTabBar()
{
    Q_D(ElaTabBar);
    delete d->_style;
}

void ElaTabBar::setTabText(int index, const QString& text)
{
    QTabBar::setTabText(index, text);
    Q_EMIT tabBarTextChanged(index, text);
}

void ElaTabBar::setTabSize(QSize tabSize)
{
    Q_D(ElaTabBar);
    d->_style->setTabSize(tabSize);
}

QSize ElaTabBar::getTabSize() const
{
    Q_D(const ElaTabBar);
    return d->_style->getTabSize();
}

QSize ElaTabBar::sizeHint() const
{
    QSize oldSize = QTabBar::sizeHint();
    QSize newSize = oldSize;
    newSize.setWidth(parentWidget()->maximumWidth());
    return oldSize.expandedTo(newSize);
}

QSize ElaTabBar::minimumSizeHint() const
{
    // useScrollButtons=false 时基类会返回全部 tab 宽度总和，会撑大窗口，宽度最小设为 0 由布局分配
    return {0, QTabBar::minimumSizeHint().height()};
}

void ElaTabBar::tabInserted(int index)
{
    Q_D(ElaTabBar);
    // 基类 addTab 内部先执行 refresh/makeVisible 再调用本虚函数，
    // _tabBarPrivate->scrollOffset 已被重置，动画状态仍是插入前的值
    qreal preScrollOffset = d->getScrollOffset();
    QTabBar::tabInserted(index);
    d->restoreScrollOffset(preScrollOffset);
}

void ElaTabBar::tabRemoved(int index)
{
    Q_D(ElaTabBar);
    // 记录移除前的偏移，基类 removeTab 的 refresh/makeVisible 会把偏移重置回最左
    qreal preScrollOffset = d->getScrollOffset();
    QTabBar::tabRemoved(index);
    // 保留拖出前的偏移，但不超过移除后的最大偏移
    d->restoreScrollOffset(preScrollOffset);
}

void ElaTabBar::mouseMoveEvent(QMouseEvent* event)
{
    QTabBar::mouseMoveEvent(event);
    Q_D(ElaTabBar);
    if (d->_tabBarPrivate->pressedIndex >= 0)
    {
        QPoint currentPos = event->pos();
        if (objectName() == "ElaCustomTabBar" && count() == 1)
        {
            if (!d->_mimeData)
            {
                d->_mimeData = new QMimeData();
                d->_mimeData->setProperty("DragType", "ElaTabBarDrag");
                d->_mimeData->setProperty("ElaTabBarObject", QVariant::fromValue(this));
                d->_mimeData->setProperty("TabSize", d->_style->getTabSize());
                d->_mimeData->setProperty("IsFloatWidget", true);
                QRect currentTabRect = tabRect(currentIndex());
                d->_mimeData->setProperty("DragPos", QPoint(currentPos.x() - currentTabRect.x(), currentPos.y() - currentTabRect.y()));
                Q_EMIT tabDragCreate(d->_mimeData);
                d->_mimeData = nullptr;
            }
        }
        else
        {
            auto& pressTabData = d->_tabBarPrivate->tabList[d->_tabBarPrivate->pressedIndex];
            QRect firstTabRect = tabRect(0);
#if (QT_VERSION > QT_VERSION_CHECK(6, 0, 0))
            QRect pressTabRect = pressTabData->rect;
            if (pressTabRect.right() + pressTabData->dragOffset > width() - firstTabRect.x())
            {
                pressTabData->dragOffset = width() - pressTabRect.right() - firstTabRect.x();
            }
            if (pressTabRect.x() + pressTabData->dragOffset < -firstTabRect.x())
            {
                pressTabData->dragOffset = -pressTabRect.x() - firstTabRect.x();
            }
#else
            QRect pressTabRect = pressTabData.rect;
            if (pressTabRect.right() + pressTabData.dragOffset > width() - firstTabRect.x())
            {
                pressTabData.dragOffset = width() - pressTabRect.right() - firstTabRect.x();
            }
            if (pressTabRect.x() + pressTabData.dragOffset < -firstTabRect.x())
            {
                pressTabData.dragOffset = -pressTabRect.x() - firstTabRect.x();
            }
#endif

            QRect moveRect = rect();
            moveRect.adjust(0, -height(), 0, height());
            if (currentPos.x() < 0 || currentPos.x() > width() || currentPos.y() > moveRect.bottom() || currentPos.y() < moveRect.y())
            {
                if (!d->_mimeData)
                {
                    d->_mimeData = new QMimeData();
                    d->_mimeData->setProperty("DragType", "ElaTabBarDrag");
                    d->_mimeData->setProperty("ElaTabBarObject", QVariant::fromValue(this));
                    d->_mimeData->setProperty("TabSize", d->_style->getTabSize());
                    Q_EMIT tabDragCreate(d->_mimeData);
                    d->_mimeData = nullptr;
                }
            }
        }
    }
}

void ElaTabBar::dragEnterEvent(QDragEnterEvent* event)
{
    Q_D(ElaTabBar);
    if (event->mimeData()->property("DragType").toString() == "ElaTabBarDrag")
    {
        event->acceptProposedAction();
        auto mimeData = const_cast<QMimeData*>(event->mimeData());
        d->_mimeData = mimeData;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        mimeData->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
        mimeData->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
        Q_EMIT tabDragEnter(mimeData);
        qApp->processEvents();
        QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(tabRect(currentIndex()).x() + d->_style->getTabSize().width() / 2, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QApplication::sendEvent(this, &pressEvent);
        QMouseEvent moveEvent(QEvent::MouseMove, QPoint(event->pos().x(), 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QApplication::sendEvent(this, &moveEvent);
    }
    QTabBar::dragEnterEvent(event);
}

void ElaTabBar::dragMoveEvent(QDragMoveEvent* event)
{
    Q_D(ElaTabBar);
    if (event->mimeData()->property("DragType").toString() == "ElaTabBarDrag")
    {
        QMouseEvent moveEvent(QEvent::MouseMove, QPoint(event->pos().x(), 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QApplication::sendEvent(this, &moveEvent);
    }
    QWidget::dragMoveEvent(event);
}

void ElaTabBar::dragLeaveEvent(QDragLeaveEvent* event)
{
    Q_D(ElaTabBar);
    if (d->_mimeData)
    {
        Q_EMIT tabDragLeave(d->_mimeData);
        d->_mimeData = nullptr;
    }
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this, &releaseEvent);
    QTabBar::dragLeaveEvent(event);
}

void ElaTabBar::dropEvent(QDropEvent* event)
{
    Q_D(ElaTabBar);
    d->_mimeData = nullptr;
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this, &releaseEvent);
    if (objectName() != "ElaCustomTabBar")
    {
        QMimeData* data = const_cast<QMimeData*>(event->mimeData());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        data->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
        data->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
        Q_EMIT tabDragDrop(data);
    }
    QTabBar::dropEvent(event);
}

void ElaTabBar::wheelEvent(QWheelEvent* event)
{
    Q_D(ElaTabBar);
    // 滚轮平滑横向滚动，不切换 tab
    int maxOffset = qMax(0, d->_tabBarPrivate->tabList.size() * d->_style->getTabSize().width() - width());
    int step = qMax(1, d->_style->getTabSize().width() / 4);
    d->setTargetScrollOffset(qBound(0.0, d->getTargetScrollOffset() - event->angleDelta().y() / 120.0 * step, qreal(maxOffset)));
    d->startScrollAnimation();
    event->accept();
}

void ElaTabBar::resizeEvent(QResizeEvent* event)
{
    QTabBar::resizeEvent(event);
    Q_D(ElaTabBar);
    // 窗口变宽时收敛超出的滚动偏移
    int maxOffset = qMax(0, d->_tabBarPrivate->tabList.size() * d->_style->getTabSize().width() - width());
    d->setTargetScrollOffset(qMin(d->getTargetScrollOffset(), qreal(maxOffset)));
    d->setScrollOffset(qMin(d->getScrollOffset(), qreal(maxOffset)));
    d->_tabBarPrivate->scrollOffset = qRound(d->getScrollOffset());
}

void ElaTabBar::paintEvent(QPaintEvent* event)
{
    Q_D(ElaTabBar);
    QSize tabSize = d->_style->getTabSize();
    for (int i = 0; i < d->_tabBarPrivate->tabList.size(); i++)
    {
#if (QT_VERSION > QT_VERSION_CHECK(6, 0, 0))
        d->_tabBarPrivate->tabList[i]->rect = QRect(tabSize.width() * i, d->_tabBarPrivate->tabList[i]->rect.y(), tabSize.width(), tabSize.height());
#else
        d->_tabBarPrivate->tabList[i].rect = QRect(tabSize.width() * i, d->_tabBarPrivate->tabList[i].rect.y(), tabSize.width(), tabSize.height());
#endif
    }
    d->_tabBarPrivate->layoutWidgets();
    QTabBar::paintEvent(event);
}