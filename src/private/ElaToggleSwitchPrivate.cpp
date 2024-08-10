#include "ElaToggleSwitchPrivate.h"

#include <QPropertyAnimation>

#include "ElaToggleSwitch.h"
ElaToggleSwitchPrivate::ElaToggleSwitchPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaToggleSwitchPrivate::~ElaToggleSwitchPrivate()
{
}

void ElaToggleSwitchPrivate::_startPosAnimation(qreal startX, qreal endX, bool isToggle)
{
    Q_Q(ElaToggleSwitch);
    QPropertyAnimation* circleAnimation = new QPropertyAnimation(q, "circleCenterX");
    connect(circleAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant& value) {
                this->_circleCenterX = value.toReal();
                q->update(); });
    circleAnimation->setStartValue(startX);
    circleAnimation->setEndValue(endX);
    circleAnimation->setEasingCurve(QEasingCurve::InOutSine);
    circleAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    _isToggled = isToggle;
    Q_EMIT q->toggled(isToggle);
}

void ElaToggleSwitchPrivate::_startRadiusAnimation(qreal startRadius, qreal endRadius)
{
    Q_Q(ElaToggleSwitch);
    QPropertyAnimation* circleRadiusAnimation = new QPropertyAnimation(q, "circleRadius");
    connect(circleRadiusAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant& value) {
        this->_circleRadius = value.toReal();
        q->update(); });
    circleRadiusAnimation->setEasingCurve(QEasingCurve::InOutSine);
    circleRadiusAnimation->setStartValue(startRadius);
    circleRadiusAnimation->setEndValue(endRadius);
    circleRadiusAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaToggleSwitchPrivate::_adjustCircleCenterX()
{
    Q_Q(ElaToggleSwitch);
    if (_circleCenterX > q->width() - q->height() / 2 - _margin * 2)
    {
        _circleCenterX = q->width() - q->height() / 2 - _margin * 2;
    }
    if (_circleCenterX < q->height() / 2)
    {
        _circleCenterX = q->height() / 2;
    }
}
