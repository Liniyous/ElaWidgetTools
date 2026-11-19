#include "ElaToggleSwitchPrivate.h"

#include "ElaToggleSwitch.h"

#include <QPropertyAnimation>

ElaToggleSwitchPrivate::ElaToggleSwitchPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaToggleSwitchPrivate::~ElaToggleSwitchPrivate()
{
}

int ElaToggleSwitchPrivate::_getTrackHeight() const
{
    Q_Q(const ElaToggleSwitch);
    return qMin(22, q->height());
}

void ElaToggleSwitchPrivate::_startPosAnimation(qreal startX, qreal endX, bool isToggle)
{
    Q_Q(ElaToggleSwitch);
    if (!_positionAnimation)
    {
        _positionAnimation = new QPropertyAnimation(q, "circleCenterX", this);
        _positionAnimation->setEasingCurve(QEasingCurve::InOutSine);
        connect(_positionAnimation, &QPropertyAnimation::valueChanged, q, [this, q](const QVariant& value) {
            _circleCenterX = value.toReal();
            q->update();
        });
    }
    _positionAnimation->stop();
    _positionAnimation->setStartValue(startX);
    _positionAnimation->setEndValue(endX);
    _positionAnimation->start();
    if (_isToggled != isToggle)
    {
        _isToggled = isToggle;
        Q_EMIT q->toggled(isToggle);
    }
}

void ElaToggleSwitchPrivate::_startRadiusAnimation(qreal startRadius, qreal endRadius)
{
    Q_Q(ElaToggleSwitch);
    QPropertyAnimation* circleRadiusAnimation = new QPropertyAnimation(q, "circleRadius");
    connect(circleRadiusAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant& value) {
        this->_circleRadius = value.toReal();
        q->update();
    });
    circleRadiusAnimation->setEasingCurve(QEasingCurve::InOutSine);
    circleRadiusAnimation->setStartValue(startRadius);
    circleRadiusAnimation->setEndValue(endRadius);
    circleRadiusAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaToggleSwitchPrivate::_adjustCircleCenterX()
{
    Q_Q(ElaToggleSwitch);
    const int trackHeight = _getTrackHeight();
    if (_circleCenterX > q->width() - trackHeight / 2 - _margin * 2)
    {
        _circleCenterX = q->width() - trackHeight / 2 - _margin * 2;
    }
    if (_circleCenterX < trackHeight / 2)
    {
        _circleCenterX = trackHeight / 2;
    }
}
