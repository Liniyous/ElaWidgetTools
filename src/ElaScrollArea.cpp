#include "ElaScrollArea.h"

#include <QScrollBar>
#include <QScroller>

#include "ElaScrollAreaPrivate.h"
#include "ElaScrollBar.h"

ElaScrollArea::ElaScrollArea(QWidget* parent)
    : QScrollArea(parent), d_ptr(new ElaScrollAreaPrivate())
{
    Q_D(ElaScrollArea);
    d->q_ptr = this;
    setObjectName("ElaScrollArea");
    setStyleSheet("#ElaScrollArea{background-color:transparent;border:0px;}");
    setHorizontalScrollBar(new ElaScrollBar(this));
    setVerticalScrollBar(new ElaScrollBar(this));
    QScrollArea::setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollArea::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

ElaScrollArea::~ElaScrollArea()
{
}

void ElaScrollArea::setIsGrabGesture(bool isEnable, qreal mousePressEventDelay)
{
    if (isEnable)
    {
        QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
        QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
        properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, mousePressEventDelay);
        properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
        properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
        properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
        QScroller::scroller(this->viewport())->setScrollerProperties(properties);
    }
    else
    {
        QScroller::ungrabGesture(this->viewport());
    }
}

void ElaScrollArea::setIsOverShoot(Qt::Orientation orientation, bool isEnable)
{
    QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
    properties.setScrollMetric(orientation == Qt::Horizontal ? QScrollerProperties::HorizontalOvershootPolicy : QScrollerProperties::VerticalOvershootPolicy, isEnable ? QScrollerProperties::OvershootAlwaysOn : QScrollerProperties::OvershootAlwaysOff);
    QScroller::scroller(this->viewport())->setScrollerProperties(properties);
}

bool ElaScrollArea::getIsOverShoot(Qt::Orientation orientation) const
{
    QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
    return properties.scrollMetric(orientation == Qt::Horizontal ? QScrollerProperties::HorizontalOvershootPolicy : QScrollerProperties::VerticalOvershootPolicy).toBool();
}

void ElaScrollArea::setIsAnimation(Qt::Orientation orientation, bool isAnimation)
{
    if (orientation == Qt::Horizontal)
    {
        dynamic_cast<ElaScrollBar*>(this->horizontalScrollBar())->setIsAnimation(isAnimation);
    }
    else
    {
        dynamic_cast<ElaScrollBar*>(this->verticalScrollBar())->setIsAnimation(isAnimation);
    }
}

bool ElaScrollArea::getIsAnimation(Qt::Orientation orientation) const
{
    if (orientation == Qt::Horizontal)
    {
        return dynamic_cast<ElaScrollBar*>(this->horizontalScrollBar())->getIsAnimation();
    }
    else
    {
        return dynamic_cast<ElaScrollBar*>(this->verticalScrollBar())->getIsAnimation();
    }
}
