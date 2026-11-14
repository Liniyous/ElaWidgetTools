#include "ElaScrollBar.h"

#include "ElaMenu.h"
#include "ElaScrollAnimation.h"
#include "ElaScrollBarStyle.h"
#include "private/ElaScrollBarPrivate.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QPointer>
#include <QScopedValueRollback>
#include <QTimer>
#include <QWheelEvent>

ELA_PROPERTY_Q_CPP(ElaScrollBar, bool, IsAnimation)
ELA_PROPERTY_Q_CPP(ElaScrollBar, qreal, SpeedLimit)
ElaScrollBar::ElaScrollBar(QWidget* parent)
    : QScrollBar(parent), d_ptr(new ElaScrollBarPrivate())
{
    Q_D(ElaScrollBar);
    d->q_ptr = this;
    setSingleStep(1);
    setObjectName("ElaScrollBar");
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    d->_pSpeedLimit = 20;
    d->_pTargetMaximum = 0;
    d->_pIsAnimation = false;
    d->_scrollAnimation = new ElaScrollAnimation(this);
    connect(this, &ElaScrollBar::rangeChanged, d, &ElaScrollBarPrivate::onRangeChanged);
    ElaScrollBarStyle* scrollBarStyle = new ElaScrollBarStyle(style());
    d->_scrollBarStyle = scrollBarStyle;
    scrollBarStyle->setParent(this);
    scrollBarStyle->setScrollBar(this);
    setStyle(scrollBarStyle);
    connect(d->_scrollAnimation, &ElaScrollAnimation::positionChanged, this, [this, d](const QPointF& position) {
        const QScopedValueRollback<bool> updating(d->_isUpdatingScroll, true);
        setValue(qRound(position.x() / d->_scrollScale));
    });
    connect(this, &QScrollBar::valueChanged, this, [d]() {
        if (!d->_isUpdatingScroll)
        {
            d->_scrollAnimation->stop();
            d->_pixelRemainder = 0;
        }
    });
    connect(this, &QScrollBar::actionTriggered, this, [this, d](int action) {
        if (d->_isSmoothScroll && action >= QAbstractSlider::SliderSingleStepAdd && action <= QAbstractSlider::SliderPageStepSub)
        {
            const int movement = sliderPosition() - value();
            setSliderPosition(value());
            d->_scroll(movement);
        }
    });

    d->_expandTimer = new QTimer(this);
    d->_expandTimer->setSingleShot(true);
    connect(d->_expandTimer, &QTimer::timeout, this, [=]() {
        d->_isExpand = underMouse() || isSliderDown();
        scrollBarStyle->startExpandAnimation(d->_isExpand);
    });
}

ElaScrollBar::ElaScrollBar(Qt::Orientation orientation, QWidget* parent)
    : ElaScrollBar(parent)
{
    setOrientation(orientation);
}

ElaScrollBar::ElaScrollBar(QScrollBar* originScrollBar, QAbstractScrollArea* parent)
    : ElaScrollBar(parent)
{
    Q_D(ElaScrollBar);
    if (!originScrollBar || !parent)
    {
        qCritical() << "原始滚动条或父控件无效";
        return;
    }
    d->_originScrollArea = parent;
    Qt::Orientation orientation = originScrollBar->orientation();
    setOrientation(orientation);
    orientation == Qt::Horizontal ? parent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff) : parent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    parent->installEventFilter(this);

    d->_originScrollBar = originScrollBar;
    d->_initAllConfig();

    connect(d->_originScrollBar, &QScrollBar::valueChanged, this, [=](int value) {
        d->_handleScrollBarValueChanged(this, value);
    });
    connect(this, &QScrollBar::valueChanged, this, [=](int value) {
        d->_handleScrollBarValueChanged(d->_originScrollBar, value);
    });
    connect(d->_originScrollBar, &QScrollBar::rangeChanged, this, [=](int min, int max) {
        d->_handleScrollBarRangeChanged(min, max);
    });
}

ElaScrollBar::~ElaScrollBar() = default;

void ElaScrollBar::setScrollBarWidth(int scrollBarWidth)
{
    Q_D(ElaScrollBar);
    scrollBarWidth = qMax(6, scrollBarWidth);
    if (d->_pScrollBarWidth == scrollBarWidth)
    {
        return;
    }
    d->_pScrollBarWidth = scrollBarWidth;
    d->_scrollBarStyle->startExpandAnimation(d->_isExpand);
    updateGeometry();
    if (d->_originScrollArea)
    {
        d->_handleScrollBarGeometry();
    }
    update();
    Q_EMIT pScrollBarWidthChanged();
}

int ElaScrollBar::getScrollBarWidth() const
{
    Q_D(const ElaScrollBar);
    return d->_pScrollBarWidth;
}

void ElaScrollBar::setIsSmoothScroll(bool isSmoothScroll)
{
    Q_D(ElaScrollBar);
    d->_isSmoothScroll = isSmoothScroll;
    if (!isSmoothScroll)
    {
        stopSmoothScroll();
    }
}

bool ElaScrollBar::getIsSmoothScroll() const
{
    Q_D(const ElaScrollBar);
    return d->_isSmoothScroll;
}

void ElaScrollBar::scrollTo(int position)
{
    Q_D(ElaScrollBar);
    stopSmoothScroll();
    if (d->_isSmoothScroll)
    {
        d->_scroll(static_cast<qreal>(position) - value());
    }
    else
    {
        setValue(position);
    }
}

void ElaScrollBar::stopSmoothScroll()
{
    Q_D(ElaScrollBar);
    d->_scrollAnimation->stop();
    d->_pixelRemainder = 0;
}

bool ElaScrollBar::event(QEvent* event)
{
    Q_D(ElaScrollBar);
    switch (event->type())
    {
    case QEvent::Enter:
    {
        d->_expandTimer->stop();
        if (!d->_isExpand)
        {
            d->_expandTimer->start(400);
        }
        break;
    }
    case QEvent::Leave:
    {
        d->_expandTimer->stop();
        if (d->_isExpand && !isSliderDown())
        {
            d->_expandTimer->start(500);
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return QScrollBar::event(event);
}

bool ElaScrollBar::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(ElaScrollBar);
    switch (event->type())
    {
    case QEvent::Show:
    case QEvent::Resize:
    case QEvent::LayoutRequest:
    {
        d->_handleScrollBarGeometry();
        break;
    }
    default:
    {
        break;
    }
    }
    return QScrollBar::eventFilter(watched, event);
}

void ElaScrollBar::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaScrollBar);
    d->_scrollAnimation->stop();
    d->_expandTimer->stop();
    d->_isExpand = true;
    d->_scrollBarStyle->startExpandAnimation(true);
    QScrollBar::mousePressEvent(event);
}

void ElaScrollBar::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaScrollBar);
    QScrollBar::mouseReleaseEvent(event);
    if (!underMouse())
    {
        d->_expandTimer->start(500);
    }
}

void ElaScrollBar::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaScrollBar);
    if (isSliderDown())
    {
        d->_scrollAnimation->stop();
    }
    QScrollBar::mouseMoveEvent(event);
}

void ElaScrollBar::wheelEvent(QWheelEvent* event)
{
    Q_D(ElaScrollBar);
    if (!d->_isSmoothScroll)
    {
        QScrollBar::wheelEvent(event);
        return;
    }
    const bool pixelInput = !event->pixelDelta().isNull();
    const auto delta = pixelInput ? event->pixelDelta() : event->angleDelta();
    const int movement = orientation() == Qt::Horizontal && delta.x() != 0 ? delta.x() : delta.y();
    if (movement == 0)
    {
        event->ignore();
        return;
    }
    if (pixelInput)
    {
        d->_scrollAnimation->stop();
        const QScopedValueRollback<bool> updating(d->_isUpdatingScroll, true);
        d->_pixelRemainder -= movement / d->_getScrollScale();
        const int steps = static_cast<int>(d->_pixelRemainder);
        d->_pixelRemainder -= steps;
        const int previous = value();
        setValue(previous + steps);
        event->setAccepted(value() != previous || (movement < 0 ? value() < maximum() : value() > minimum()));
    }
    else
    {
        const qreal steps = -movement / 120.0;
        const bool pageScroll = event->modifiers().testFlag(Qt::ControlModifier) || event->modifiers().testFlag(Qt::ShiftModifier);
        const qreal distance = pageScroll ? qBound(-1.0, steps, 1.0) * pageStep() : steps * QApplication::wheelScrollLines() * singleStep();
        event->setAccepted(d->_scroll(distance));
    }
}

void ElaScrollBar::contextMenuEvent(QContextMenuEvent* event)
{
    Q_D(ElaScrollBar);
    d->_scrollAnimation->stop();
    bool horiz = this->orientation() == Qt::Horizontal;
    QPointer<ElaMenu> menu = new ElaMenu(this);
    menu->setMenuItemHeight(27);
    // Scroll here
    QAction* actScrollHere = menu->addElaIconAction(ElaIconType::UpDownLeftRight, tr("滚动到此处"));
    menu->addSeparator();
    // Left edge Top
    QAction* actScrollTop = menu->addElaIconAction(horiz ? ElaIconType::ArrowLeftToLine : ElaIconType::ArrowUpToLine, horiz ? tr("左边缘") : tr("顶端"));
    // Right edge Bottom
    QAction* actScrollBottom = menu->addElaIconAction(horiz ? ElaIconType::ArrowRightToLine : ElaIconType::ArrowDownToLine, horiz ? tr("右边缘") : tr("底部"));
    menu->addSeparator();
    // Page left Page up
    QAction* actPageUp = menu->addElaIconAction(horiz ? ElaIconType::AnglesLeft : ElaIconType::AnglesUp, horiz ? tr("向左翻页") : tr("向上翻页"));
    //Page right Page down
    QAction* actPageDn = menu->addElaIconAction(horiz ? ElaIconType::AnglesRight : ElaIconType::AnglesDown, horiz ? tr("向右翻页") : tr("向下翻页"));
    menu->addSeparator();
    //Scroll left Scroll up
    QAction* actScrollUp = menu->addElaIconAction(horiz ? ElaIconType::AngleLeft : ElaIconType::AngleUp, horiz ? tr("向左滚动") : tr("向上滚动"));
    //Scroll right Scroll down
    QAction* actScrollDn = menu->addElaIconAction(horiz ? ElaIconType::AngleRight : ElaIconType::AngleDown, horiz ? tr("向右滚动") : tr("向下滚动"));
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
