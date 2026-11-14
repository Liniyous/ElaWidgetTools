#include "ElaTabBarPrivate.h"

#include "ElaScrollBar.h"
#include "ElaTabBar.h"
#include "ElaTabBarStyle.h"

#include <QSignalBlocker>
#include <private/qtabbar_p.h>

ElaTabBarPrivate::ElaTabBarPrivate(QObject* parent)
    : QObject(parent)
{
}

void ElaTabBarPrivate::onScrollBarValueChanged(int position)
{
    Q_Q(ElaTabBar);
    _tabBarPrivate->scrollOffset = position;
    _tabBarPrivate->layoutWidgets();
    q->update();
}

void ElaTabBarPrivate::updateScrollRange()
{
    Q_Q(ElaTabBar);
    const int contentWidth = q->count() > 0 ? q->tabRect(q->count() - 1).right() + _tabBarPrivate->scrollOffset + 1 : 0;
    // 滚动条独占顶部空间 标签绘制和命中区域保持一致
    for (int index = 0; index < q->count(); ++index)
    {
        _tabBarPrivate->at(index)->rect.moveTop(_scrollBar->height());
    }
    _tabBarPrivate->layoutWidgets();
    _scrollBar->setGeometry(0, 0, q->width(), _scrollBar->height());
    _scrollBar->setPageStep(q->width());
    _scrollBar->setSingleStep(qMax(1, _style->getTabSize().width() / 4));
    _scrollBar->setRange(0, qMax(0, contentWidth - q->width()));
    _scrollBar->setVisible(_scrollBar->maximum() > 0);
    _scrollBar->raise();
    setScrollPosition(_scrollBar->value(), false);
}

void ElaTabBarPrivate::setScrollPosition(int position, bool animated)
{
    if (animated)
    {
        _scrollBar->scrollTo(position);
    }
    else
    {
        _scrollBar->stopSmoothScroll();
        const QSignalBlocker blocker(_scrollBar);
        _scrollBar->setValue(position);
        onScrollBarValueChanged(_scrollBar->value());
    }
}

void ElaTabBarPrivate::ensureTabVisible(int index)
{
    Q_Q(ElaTabBar);
    setScrollPosition(_scrollBar->value(), false);
    if (index < 0)
    {
        return;
    }
    const auto tab = q->tabRect(index);
    if (tab.left() < 0 || tab.width() > q->width())
    {
        setScrollPosition(_scrollBar->value() + tab.left(), true);
    }
    else if (tab.right() >= q->width())
    {
        setScrollPosition(_scrollBar->value() + tab.right() - q->width() + 1, true);
    }
}

int ElaTabBarPrivate::getDropIndex(const QPoint& position) const
{
    Q_Q(const ElaTabBar);
    for (int index = 0; index < q->count(); ++index)
    {
        if (position.x() < q->tabRect(index).center().x())
        {
            return index;
        }
    }
    return q->count();
}

void ElaTabBarPrivate::updateDropPosition(const QPoint& position)
{
    Q_Q(ElaTabBar);
    _dropPosition = position;
    _dropIndex = getDropIndex(position);
    _dragScrollDirection = position.x() < 28 ? -1 : (position.x() > q->width() - 28 ? 1 : 0);
    q->update();
}

void ElaTabBarPrivate::clearDropPosition()
{
    Q_Q(ElaTabBar);
    _dropIndex = -1;
    _dragScrollDirection = 0;
    q->update();
}
