#include "ElaToolButton.h"

#include "ElaControlAnimation.h"
#include "ElaIcon.h"
#include "ElaMenu.h"
#include "ElaToolButtonPrivate.h"
#include "ElaToolButtonStyle.h"

#include <QEvent>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStyleOptionToolButton>
#include <QTimer>

ElaToolButton::ElaToolButton(QWidget* parent)
    : QToolButton(parent), d_ptr(new ElaToolButtonPrivate())
{
    Q_D(ElaToolButton);
    d->q_ptr = this;
    setObjectName("ElaToolButton");
    setStyleSheet("#ElaToolButton{background-color:transparent;}");
    setIconSize(QSize(22, 22));
    setPopupMode(QToolButton::InstantPopup);
    d->_toolButtonStyle = new ElaToolButtonStyle(style());
    d->_toolButtonStyle->setParent(this);
    setStyle(d->_toolButtonStyle);
    d->_controlAnimation = new ElaControlAnimation(this);
    d->_toolButtonStyle->setControlAnimation(d->_controlAnimation);
    d->_menuDelayTimer = new QTimer(d);
    d->_menuDelayTimer->setSingleShot(true);
    d->_menuDelayTimer->setTimerType(Qt::PreciseTimer);
    d->_menuDelayTimer->setInterval(83);
    connect(d->_menuDelayTimer, &QTimer::timeout, d, &ElaToolButtonPrivate::onMenuDelayTimeout);
}

ElaToolButton::~ElaToolButton() = default;

void ElaToolButton::setBorderRadius(int borderRadius)
{
    Q_D(ElaToolButton);
    d->_toolButtonStyle->setBorderRadius(borderRadius);
    update();
    Q_EMIT pBorderRadiusChanged();
}

int ElaToolButton::getBorderRadius() const
{
    Q_D(const ElaToolButton);
    return d->_toolButtonStyle->getBorderRadius();
}

void ElaToolButton::setIsSelected(bool isSelected)
{
    Q_D(ElaToolButton);
    d->_toolButtonStyle->setIsSelected(isSelected);
    update();
    Q_EMIT pIsSelectedChanged();
}

bool ElaToolButton::getIsSelected() const
{
    Q_D(const ElaToolButton);
    return d->_toolButtonStyle->getIsSelected();
}

void ElaToolButton::setIsTransparent(bool isTransparent)
{
    Q_D(ElaToolButton);
    d->_toolButtonStyle->setIsTransparent(isTransparent);
    update();
}

bool ElaToolButton::getIsTransparent() const
{
    Q_D(const ElaToolButton);
    return d->_toolButtonStyle->getIsTransparent();
}

void ElaToolButton::setMenu(ElaMenu* menu)
{
    if (menu == this->menu())
    {
        return;
    }
    if (this->menu())
    {
        this->menu()->removeEventFilter(this);
    }
    d_ptr->_cancelMenu();
    QToolButton::setMenu(menu);
    if (menu)
    {
        menu->setMenuItemHeight(32);
        menu->installEventFilter(this);
    }
    d_ptr->_controlAnimation->release();
    updateGeometry();
    update();
}

void ElaToolButton::setElaIcon(ElaIconType::IconName icon)
{
    setProperty("ElaIconType", QChar(icon));
    setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Broom, 1));
}

void ElaToolButton::setElaIcon(ElaIconType::IconName icon, int rotate)
{
    setElaIcon(icon);
    setProperty("ElaIconRotate", rotate);
}

bool ElaToolButton::event(QEvent* event)
{
    Q_D(ElaToolButton);
    if (d->_menuDelayTimer && (event->type() == QEvent::Hide || event->type() == QEvent::WindowDeactivate || (event->type() == QEvent::EnabledChange && !isEnabled()) || (event->type() == QEvent::FocusOut && static_cast<QFocusEvent*>(event)->reason() != Qt::PopupFocusReason)))
    {
        d->_cancelMenu();
    }
    return QToolButton::event(event);
}

void ElaToolButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaToolButton);
    if (event->button() == Qt::LeftButton && d->_getIsMenuButtonHit(event->pos()))
    {
        d->_menuDelayTimer->stop();
        d->_isMenuPress = true;
        setDown(true);
        event->accept();
        return;
    }
    QToolButton::mousePressEvent(event);
}

void ElaToolButton::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaToolButton);
    if (d->_isMenuPress)
    {
        setDown(d->_getIsMenuButtonHit(event->pos()));
        event->accept();
        return;
    }
    QToolButton::mouseMoveEvent(event);
}

void ElaToolButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaToolButton);
    if (event->button() == Qt::LeftButton && d->_isMenuPress)
    {
        d->_isMenuPress = false;
        setDown(false);
        if (d->_getIsMenuButtonHit(event->pos()))
        {
            d->_queueMenu();
        }
        event->accept();
        return;
    }
    QToolButton::mouseReleaseEvent(event);
}

void ElaToolButton::keyPressEvent(QKeyEvent* event)
{
    Q_D(ElaToolButton);
    if (event->key() == Qt::Key_Escape && (d->_menuDelayTimer->isActive() || d->_isMenuPress || d->_menuPressKey))
    {
        d->_cancelMenu();
        event->accept();
        return;
    }
    if (menu() && isEnabled() && popupMode() == QToolButton::InstantPopup
        && (event->key() == Qt::Key_Space || event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Down || event->key() == Qt::Key_F4))
    {
        if (!event->isAutoRepeat())
        {
            d->_menuPressKey = event->key();
            setDown(true);
        }
        event->accept();
        return;
    }
    QToolButton::keyPressEvent(event);
}

void ElaToolButton::keyReleaseEvent(QKeyEvent* event)
{
    Q_D(ElaToolButton);
    if (d->_menuPressKey && event->key() == d->_menuPressKey)
    {
        if (!event->isAutoRepeat())
        {
            d->_menuPressKey = 0;
            setDown(false);
            d->_queueMenu();
        }
        event->accept();
        return;
    }
    QToolButton::keyReleaseEvent(event);
}

bool ElaToolButton::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(ElaToolButton);
    if (watched == menu() && event->type() == QEvent::Show)
    {
        QStyleOptionToolButton option;
        initStyleOption(&option);
        if (option.state.testFlag(QStyle::State_Sunken))
        {
            d->_positionMenu();
        }
    }
    if (watched == menu() && (event->type() == QEvent::Hide || event->type() == QEvent::MouseButtonRelease))
    {
        d->_controlAnimation->release();
        update();
    }
    return QToolButton::eventFilter(watched, event);
}
