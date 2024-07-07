#include "ElaScrollBar.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QWheelEvent>

#include "ElaScrollBarStyle.h"
#include "private/ElaScrollBarPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaScrollBar, bool, isAnimation)
ElaScrollBar::ElaScrollBar(QWidget* parent)
    : QScrollBar(parent), d_ptr(new ElaScrollBarPrivate())
{
    Q_D(ElaScrollBar);
    d->q_ptr = this;
    setSingleStep(1);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    d->_pTargetMaximum = 0;
    d->_pisAnimation = false;
    connect(this, &ElaScrollBar::rangeChanged, d, &ElaScrollBarPrivate::onRangeChanged);
    setStyle(new ElaScrollBarStyle(style()));
    d->_slideSmoothAnimation = new QPropertyAnimation(this, "value");
    d->_slideSmoothAnimation->setEasingCurve(QEasingCurve::OutSine);
    d->_slideSmoothAnimation->setDuration(300);
    connect(d->_slideSmoothAnimation, &QPropertyAnimation::finished, this, [=]() { d->_scrollValue = value(); });
}

ElaScrollBar::ElaScrollBar(Qt::Orientation orientation, QWidget* parent)
    : QScrollBar(orientation, parent), d_ptr(new ElaScrollBarPrivate())
{
    Q_D(ElaScrollBar);
    d->q_ptr = this;
    setSingleStep(1);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    d->_pTargetMaximum = 0;
    d->_pisAnimation = false;
    connect(this, &ElaScrollBar::rangeChanged, d, &ElaScrollBarPrivate::onRangeChanged);
    setStyle(new ElaScrollBarStyle(style()));
    d->_slideSmoothAnimation = new QPropertyAnimation(this, "value");
    d->_slideSmoothAnimation->setEasingCurve(QEasingCurve::OutSine);
    d->_slideSmoothAnimation->setDuration(300);
    connect(d->_slideSmoothAnimation, &QPropertyAnimation::finished, this, [=]() { d->_scrollValue = value(); });
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
