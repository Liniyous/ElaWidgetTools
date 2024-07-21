#include "ElaScrollBar.h"

#include <QPainter>
#include <QPointer>
#include <QPropertyAnimation>
#include <QWheelEvent>

#include "ElaMenu.h"
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

void ElaScrollBar::contextMenuEvent(QContextMenuEvent* event)
{
    Q_D(ElaScrollBar);
    bool horiz = this->orientation() == Qt::Horizontal;
    QPointer<ElaMenu> menu = new ElaMenu(this);
    menu->setMenuItemHeight(27);
    QAction* actScrollHere = menu->addAction(tr("Scroll here"));
    menu->addSeparator();
    QAction* actScrollTop = menu->addAction(horiz ? tr("Left edge") : tr("Top"));
    QAction* actScrollBottom = menu->addAction(horiz ? tr("Right edge") : tr("Bottom"));
    menu->addSeparator();
    QAction* actPageUp = menu->addAction(horiz ? tr("Page left") : tr("Page up"));
    QAction* actPageDn = menu->addAction(horiz ? tr("Page right") : tr("Page down"));
    menu->addSeparator();
    QAction* actScrollUp = menu->addAction(horiz ? tr("Scroll left") : tr("Scroll up"));
    QAction* actScrollDn = menu->addAction(horiz ? tr("Scroll right") : tr("Scroll down"));
    QAction* actionSelected = menu->exec(event->globalPos());
    delete menu;
    if (!actionSelected)
    {
        return;
    }
    if (actionSelected == actScrollHere)
    {
        setValue(d->_pixelPosToRangeValue(horiz ? event->pos().x() : event->pos().y()));
    }
    else if (actionSelected == actScrollTop)
    {
        triggerAction(QAbstractSlider::SliderToMinimum);
    }
    else if (actionSelected == actScrollBottom)
    {
        triggerAction(QAbstractSlider::SliderToMaximum);
    }
    else if (actionSelected == actPageUp)
    {
        triggerAction(QAbstractSlider::SliderPageStepSub);
    }
    else if (actionSelected == actPageDn)
    {
        triggerAction(QAbstractSlider::SliderPageStepAdd);
    }
    else if (actionSelected == actScrollUp)
    {
        triggerAction(QAbstractSlider::SliderSingleStepSub);
    }
    else if (actionSelected == actScrollDn)
    {
        triggerAction(QAbstractSlider::SliderSingleStepAdd);
    }
}
