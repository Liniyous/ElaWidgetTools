#include "ElaScrollArea.h"

#include <QScrollBar>
#include <QScroller>

#include "ElaScrollBar.h"
ElaScrollArea::ElaScrollArea(QWidget* parent)
    : QScrollArea(parent)
{
    setObjectName("ElaScrollArea");
    setStyleSheet("#ElaScrollArea{border-radius: 2px;background-color: transparent;border:0px;}");
    setHorizontalScrollBar(new ElaScrollBar(this));
    setVerticalScrollBar(new ElaScrollBar(this));
    QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
    QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
#if (QT_VERSION < QT_VERSION_CHECK(6, 5, 3))
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
#else
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0.5);
#endif
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    QScroller::scroller(this->viewport())->setScrollerProperties(properties);
}

ElaScrollArea::~ElaScrollArea()
{
}

void ElaScrollArea::setIsGrabGesture(bool isEnable, qreal mousePressEventDelay)
{
    if (isEnable)
    {
        QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
        properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
        properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3))
        properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, mousePressEventDelay);
#endif
        QScroller::scroller(this->viewport())->setScrollerProperties(properties);
    }
    else
    {
        QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
        properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
        properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
        QScroller::scroller(this->viewport())->setScrollerProperties(properties);
    }
}

void ElaScrollArea::setIsGrabGesture(Qt::Orientation orientationm, bool isEnable, qreal mousePressEventDelay)
{
    if (isEnable)
    {
        QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
        if (orientationm == Qt::Horizontal)
        {
            properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
            properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
        }
        else
        {
            properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
            properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
        }
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3))
        properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, mousePressEventDelay);
#endif
        QScroller::scroller(this->viewport())->setScrollerProperties(properties);
    }
    else
    {
        QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
        properties.setScrollMetric(orientationm == Qt::Horizontal ? QScrollerProperties::HorizontalOvershootPolicy : QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
        QScroller::scroller(this->viewport())->setScrollerProperties(properties);
    }
}

bool ElaScrollArea::getIsGrabGesture(Qt::Orientation orientation) const
{
    QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
    return properties.scrollMetric(orientation == Qt::Horizontal ? QScrollerProperties::HorizontalOvershootPolicy : QScrollerProperties::VerticalOvershootPolicy).toBool();
}

void ElaScrollArea::setIsAnimation(Qt::Orientation orientation, bool isAnimation)
{
    if (orientation == Qt::Horizontal)
    {
        dynamic_cast<ElaScrollBar*>(this->horizontalScrollBar())->setisAnimation(isAnimation);
    }
    else
    {
        dynamic_cast<ElaScrollBar*>(this->verticalScrollBar())->setisAnimation(isAnimation);
    }
}

bool ElaScrollArea::getIsAnimation(Qt::Orientation orientation) const
{
    if (orientation == Qt::Horizontal)
    {
        return dynamic_cast<ElaScrollBar*>(this->horizontalScrollBar())->getisAnimation();
    }
    else
    {
        return dynamic_cast<ElaScrollBar*>(this->verticalScrollBar())->getisAnimation();
    }
}
