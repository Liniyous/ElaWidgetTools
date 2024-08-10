#include "ElaToolButton.h"

#include <QDebug>
#include <QEvent>
#include <QPropertyAnimation>
#include "DeveloperComponents/ElaToolButtonStyle.h"
#include "ElaIcon.h"
#include "ElaMenu.h"
#include "ElaToolButtonPrivate.h"
ElaToolButton::ElaToolButton(QWidget* parent)
    : QToolButton(parent), d_ptr(new ElaToolButtonPrivate())
{
    Q_D(ElaToolButton);
    d->q_ptr = this;
    setIconSize(QSize(22, 22));
    setPopupMode(QToolButton::InstantPopup);
    d->_toolButtonStyle = new ElaToolButtonStyle(style());
    setStyle(d->_toolButtonStyle);
}

ElaToolButton::~ElaToolButton()
{
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
    if (!menu || menu == this->menu())
    {
        return;
    }
    menu->setMenuItemHeight(27);
    QToolButton::setMenu(menu);
    menu->installEventFilter(this);
}

void ElaToolButton::setElaIcon(ElaIconType::IconName icon)
{
    setProperty("ElaIconType", QChar((unsigned short)icon));
    setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Broom, 1));
}

bool ElaToolButton::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(ElaToolButton);
    if (watched == menu())
    {
        switch(event->type())
        {
        case QEvent::Show:
        {
            //指示器动画
            QPropertyAnimation* rotateAnimation = new QPropertyAnimation(d->_toolButtonStyle, "pExpandIconRotate");
            connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                        update();
                    });
            rotateAnimation->setDuration(300);
            rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
            rotateAnimation->setStartValue(d->_toolButtonStyle->getExpandIconRotate());
            rotateAnimation->setEndValue(-180);
            rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            break;
        }
        case QEvent::Hide:
        {
            //指示器动画
            QPropertyAnimation* rotateAnimation = new QPropertyAnimation(d->_toolButtonStyle, "pExpandIconRotate");
            connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                        update();
                    });
            rotateAnimation->setDuration(300);
            rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
            rotateAnimation->setStartValue(d->_toolButtonStyle->getExpandIconRotate());
            rotateAnimation->setEndValue(0);
            rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            break;
        }
        default:
        {
            break;
        }
        }
    }
    return QObject::eventFilter(watched, event);
}
