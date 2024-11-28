#include "ElaScrollBar.h"

#include <QDebug>
#include <QPainter>
#include <QPointer>
#include <QPropertyAnimation>
#include <QTimer>
#include <QWheelEvent>

#include "ElaMenu.h"
#include "ElaScrollBarStyle.h"
#include "private/ElaScrollBarPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaScrollBar, bool, IsAnimation)
Q_PROPERTY_CREATE_Q_CPP(ElaScrollBar, qreal, SpeedLimit)
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
    connect(this, &ElaScrollBar::rangeChanged, d, &ElaScrollBarPrivate::onRangeChanged);
    ElaScrollBarStyle* scrollBarStyle = new ElaScrollBarStyle(style());
    scrollBarStyle->setScrollBar(this);
    setStyle(scrollBarStyle);
    d->_slideSmoothAnimation = new QPropertyAnimation(this, "value");
    d->_slideSmoothAnimation->setEasingCurve(QEasingCurve::OutSine);
    d->_slideSmoothAnimation->setDuration(300);
    connect(d->_slideSmoothAnimation, &QPropertyAnimation::finished, this, [=]() { d->_scrollValue = value(); });

    d->_expandTimer = new QTimer(this);
    connect(d->_expandTimer, &QTimer::timeout, this, [=]() {
        d->_expandTimer->stop();
        d->_isExpand = underMouse();
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
        qCritical() << "Invalid origin or parent!";
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

ElaScrollBar::~ElaScrollBar()
{
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
            d->_expandTimer->start(350);
        }
        break;
    }
    case QEvent::Leave:
    {
        d->_expandTimer->stop();
        if (d->_isExpand)
        {
            d->_expandTimer->start(350);
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
    int verticalDelta = event->angleDelta().y();
    if (d->_slideSmoothAnimation->state() == QAbstractAnimation::Stopped)
    {
        d->_scrollValue = value();
    }
    if (verticalDelta != 0)
    {
        if ((value() == minimum() && verticalDelta > 0) || (value() == maximum() && verticalDelta < 0))
        {
            QScrollBar::wheelEvent(event);
            return;
        }
        d->_scroll(event->modifiers(), verticalDelta);
    }
    else
    {
        int horizontalDelta = event->angleDelta().x();
        if ((value() == minimum() && horizontalDelta > 0) || (value() == maximum() && horizontalDelta < 0))
        {
            QScrollBar::wheelEvent(event);
            return;
        }
        d->_scroll(event->modifiers(), horizontalDelta);
    }
    event->accept();
}

void ElaScrollBar::contextMenuEvent(QContextMenuEvent* event)
{
    Q_D(ElaScrollBar);
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

void ElaScrollBar::setSpace(int verSpcce, int horSpace)
{
    Q_D(ElaScrollBar);
    d->_horSpace = horSpace;
    d->_verSpace = verSpcce;
}
