#include "ElaTabBar.h"

#include "ElaScrollBar.h"
#include "ElaTabBarPrivate.h"
#include "ElaTabBarStyle.h"
#include "ElaTabDragData.h"
#include "ElaTheme.h"

#include <QCoreApplication>
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <private/qtabbar_p.h>

ElaTabBar::ElaTabBar(QWidget* parent)
    : QTabBar(parent), d_ptr(new ElaTabBarPrivate())
{
    Q_D(ElaTabBar);
    d->q_ptr = this;
    d->_tabBarPrivate = static_cast<QTabBarPrivate*>(QTabBar::d_ptr.data());
    d->_style = new ElaTabBarStyle(style());
    d->_style->setParent(this);
    d->_scrollBar = new ElaScrollBar(Qt::Horizontal, this);
    d->_scrollBar->setAccessibleName("标签滚动条");
    d->_scrollBar->setScrollBarWidth(8);
    d->_scrollBar->setFixedHeight(d->_scrollBar->sizeHint().height());
    d->_scrollBar->setIsAnimation(false);
    d->_scrollBar->hide();
    setObjectName("ElaTabBar");
    setStyleSheet("#ElaTabBar{background-color:transparent;}");
    setStyle(d->_style);
    setMouseTracking(true);
    setTabsClosable(true);
    setMovable(true);
    setExpanding(false);
    setUsesScrollButtons(false);
    setAcceptDrops(true);
    connect(d->_scrollBar, &QScrollBar::valueChanged, d, &ElaTabBarPrivate::onScrollBarValueChanged);
    connect(d->_scrollBar, &ElaScrollBar::pScrollBarWidthChanged, this, [this, d]() {
        d->_scrollBar->setFixedHeight(d->_scrollBar->sizeHint().height());
        d->updateScrollRange();
        setMinimumHeight(minimumSizeHint().height());
        updateGeometry();
    });
    connect(this, &QTabBar::currentChanged, d, &ElaTabBarPrivate::ensureTabVisible);
    d->_dragScrollTimer = new QTimer(d);
    d->_dragScrollTimer->setInterval(30);
    connect(d->_dragScrollTimer, &QTimer::timeout, d, [d]() {
        if (d->_dragScrollDirection != 0)
        {
            d->setScrollPosition(d->_scrollBar->value() + d->_dragScrollDirection * 12, false);
            d->updateDropPosition(d->_dropPosition);
        }
    });
}

ElaTabBar::~ElaTabBar() = default;

void ElaTabBar::setTabSize(QSize tabSize)
{
    Q_D(ElaTabBar);
    d->_style->setTabSize(tabSize);
    setMinimumHeight(minimumSizeHint().height());
    updateGeometry();
    // 字体变化事件让 Qt 按新的标签尺寸重新布局
    QEvent event(QEvent::FontChange);
    QTabBar::changeEvent(&event);
    Q_EMIT pTabSizeChanged();
}

QSize ElaTabBar::getTabSize() const
{
    Q_D(const ElaTabBar);
    return d->_style->getTabSize();
}

void ElaTabBar::setIsTabWidthAdaptive(bool isAdaptive)
{
    Q_D(ElaTabBar);
    if (getIsTabWidthAdaptive() == isAdaptive)
    {
        return;
    }
    d->_style->setIsTabWidthAdaptive(isAdaptive);
    QEvent event(QEvent::FontChange);
    QTabBar::changeEvent(&event);
    updateGeometry();
    Q_EMIT pIsTabWidthAdaptiveChanged();
}

bool ElaTabBar::getIsTabWidthAdaptive() const
{
    Q_D(const ElaTabBar);
    return d->_style->getIsTabWidthAdaptive();
}

void ElaTabBar::setTabText(int index, const QString& text)
{
    QTabBar::setTabText(index, text);
    Q_EMIT tabBarTextChanged(index, text);
}

void ElaTabBar::setIsActive(bool isActive)
{
    Q_D(ElaTabBar);
    d->_style->setIsActive(isActive);
    update();
}

void ElaTabBar::setIsPinned(bool isPinned)
{
    Q_D(ElaTabBar);
    d->_style->setIsPinned(isPinned);
    update();
}

void ElaTabBar::setIsDragEnabled(bool isEnabled)
{
    Q_D(ElaTabBar);
    d->_isDragEnabled = isEnabled;
}

ElaScrollBar* ElaTabBar::getScrollBar() const
{
    Q_D(const ElaTabBar);
    return d->_scrollBar;
}

void ElaTabBar::locateCurrentTab()
{
    Q_D(ElaTabBar);
    d->ensureTabVisible(currentIndex());
}

QSize ElaTabBar::sizeHint() const
{
    int width = 0;
    for (int index = 0; index < count(); ++index)
    {
        width += tabSizeHint(index).width();
    }
    return {width, minimumSizeHint().height()};
}

QSize ElaTabBar::minimumSizeHint() const
{
    Q_D(const ElaTabBar);
    return {0, getTabSize().height() + d->_scrollBar->height()};
}

QSize ElaTabBar::tabSizeHint(int index) const
{
    return getIsTabWidthAdaptive() ? QTabBar::tabSizeHint(index) : getTabSize();
}

QSize ElaTabBar::minimumTabSizeHint(int index) const
{
    return tabSizeHint(index);
}

void ElaTabBar::tabLayoutChange()
{
    QTabBar::tabLayoutChange();
    Q_D(ElaTabBar);
    d->updateScrollRange();
}

void ElaTabBar::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaTabBar);
    d->setScrollPosition(d->_scrollBar->value(), false);
    QTabBar::mousePressEvent(event);
}

void ElaTabBar::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaTabBar);
    const int index = d->_tabBarPrivate->pressedIndex;
    if (d->_isDragEnabled && index >= 0 && event->buttons().testFlag(Qt::LeftButton) && !rect().adjusted(-12, -12, 12, 12).contains(event->pos()))
    {
        QMouseEvent release(QEvent::MouseButtonRelease, event->pos(), event->globalPos(), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
        QTabBar::mouseReleaseEvent(&release);
        Q_EMIT tabDragRequested(index);
        return;
    }
    QTabBar::mouseMoveEvent(event);
}

void ElaTabBar::dragEnterEvent(QDragEnterEvent* event)
{
    if (qobject_cast<const ElaTabDragData*>(event->mimeData()))
    {
        Q_D(ElaTabBar);
        event->acceptProposedAction();
        d->updateDropPosition(event->pos());
        d->_dragScrollTimer->start();
    }
}

void ElaTabBar::dragMoveEvent(QDragMoveEvent* event)
{
    Q_D(ElaTabBar);
    d->updateDropPosition(event->pos());
    event->acceptProposedAction();
}

void ElaTabBar::dragLeaveEvent(QDragLeaveEvent* event)
{
    Q_D(ElaTabBar);
    d->_dragScrollTimer->stop();
    d->clearDropPosition();
    event->accept();
}

void ElaTabBar::dropEvent(QDropEvent* event)
{
    Q_D(ElaTabBar);
    const int index = d->getDropIndex(event->pos());
    d->_dragScrollTimer->stop();
    d->clearDropPosition();
    Q_EMIT tabDropRequested(event->mimeData(), index, event->proposedAction());
    event->acceptProposedAction();
}

void ElaTabBar::wheelEvent(QWheelEvent* event)
{
    Q_D(ElaTabBar);
    QCoreApplication::sendEvent(d->_scrollBar, event);
}

void ElaTabBar::resizeEvent(QResizeEvent* event)
{
    QTabBar::resizeEvent(event);
    Q_D(ElaTabBar);
    d->updateScrollRange();
}

void ElaTabBar::paintEvent(QPaintEvent* event)
{
    QTabBar::paintEvent(event);
    Q_D(ElaTabBar);
    if (d->_dropIndex >= 0)
    {
        const int x = d->_dropIndex < count() ? tabRect(d->_dropIndex).left() : (count() > 0 ? tabRect(count() - 1).right() + 1 : 0);
        QPainter painter(this);
        painter.fillRect(QRect(qBound(0, x, width() - 3), d->_scrollBar->height(), 3, getTabSize().height()), ElaThemeColor(eTheme->getThemeMode(), PrimaryNormal));
    }
}
