#include "ElaScrollBar.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QWheelEvent>

#include "private/ElaScrollBarPrivate.h"
ElaScrollBar::ElaScrollBar(QWidget* parent)
    : QScrollBar(parent), d_ptr(new ElaScrollBarPrivate())
{
    Q_D(ElaScrollBar);
    d->q_ptr = this;
    d->_initStyle();
}

ElaScrollBar::ElaScrollBar(Qt::Orientation orientation, QWidget* parent)
    : QScrollBar(orientation, parent), d_ptr(new ElaScrollBarPrivate())
{
    Q_D(ElaScrollBar);
    d->q_ptr = this;
    d->_initStyle();
}

ElaScrollBar::~ElaScrollBar()
{
}

void ElaScrollBar::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaScrollBar);
    d->_slideSmoothAnimation->stop();
    QScrollBar::mousePressEvent(event);
    d->_scrollValue = value();
}

void ElaScrollBar::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaScrollBar);
    d->_slideSmoothAnimation->stop();
    QScrollBar::mouseReleaseEvent(event);
    d->_scrollValue = value();
}

void ElaScrollBar::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaScrollBar);
    d->_slideSmoothAnimation->stop();
    QScrollBar::mouseMoveEvent(event);
    d->_scrollValue = value();
}

void ElaScrollBar::wheelEvent(QWheelEvent* event)
{
    Q_D(ElaScrollBar);
    if (this->orientation() == Qt::Horizontal)
    {
        int horizontalDelta = event->angleDelta().x();
        if (d->_lastHorizontalDeltaAngle != horizontalDelta)
        {
            d->_scrollValue = value();
            d->_lastHorizontalDeltaAngle = horizontalDelta;
        }
        d->_scroll(horizontalDelta);
    }
    else
    {
        int verticalDelta = event->angleDelta().y();
        if (d->_lastVerticalDeltaAngle != verticalDelta)
        {
            d->_scrollValue = value();
            d->_lastVerticalDeltaAngle = verticalDelta;
        }
        d->_scroll(verticalDelta);
    }
}

void ElaScrollBar::sliderChange(SliderChange change)
{
    Q_D(ElaScrollBar);
    if (change == SliderRangeChange && this->objectName() == "NavigationScrollBar")
    {
        if (abs(maximum() - d->_lastMaximum) > 35)
        {
            if (d->_isRangeAnimationFinished)
            {
                d->_rangeSmoothAnimation->setStartValue(d->_lastMaximum);
                d->_rangeSmoothAnimation->setEndValue(maximum());
                d->_isRangeAnimationFinished = false;
                d->_lastMaximum = maximum();
                d->_rangeSmoothAnimation->start();
                return;
            }
        }
        else
        {
            d->_lastMaximum = maximum();
        }
    }
    QScrollBar::sliderChange(change);
}
