#include "ElaScrollAnimation.h"

#include <cmath>

ElaScrollAnimation::ElaScrollAnimation(QObject* parent)
    : QAbstractAnimation(parent)
{
}

int ElaScrollAnimation::duration() const
{
    return _duration;
}

QPointF ElaScrollAnimation::getPosition() const
{
    return _position;
}

QPointF ElaScrollAnimation::getTargetPosition() const
{
    return _targetPosition;
}

bool ElaScrollAnimation::scrollBy(const QPointF& movement, const QPointF& position, const QPointF& minimum, const QPointF& maximum, qreal speedLimit)
{
    const bool running = state() == Running;
    const auto current = running ? _position : position;
    const auto pending = running ? _targetPosition : current;
    stop();
    _startPosition = current;
    _position = current;
    _targetPosition = {
        _getTarget(movement.x(), current.x(), pending.x(), minimum.x(), maximum.x(), speedLimit),
        _getTarget(movement.y(), current.y(), pending.y(), minimum.y(), maximum.y(), speedLimit)};
    _duration = static_cast<int>(std::ceil(1000 * qMax(_getDuration(_targetPosition.x() - current.x()), _getDuration(_targetPosition.y() - current.y()))));
    if (_duration == 0)
    {
        return false;
    }
    start();
    return true;
}

void ElaScrollAnimation::updateCurrentTime(int currentTime)
{
    const qreal elapsed = currentTime / 1000.0;
    _position = {_getPosition(_startPosition.x(), _targetPosition.x(), elapsed), _getPosition(_startPosition.y(), _targetPosition.y(), elapsed)};
    Q_EMIT positionChanged(_position);
}

qreal ElaScrollAnimation::_getTarget(qreal movement, qreal current, qreal pending, qreal minimum, qreal maximum, qreal speedLimit)
{
    if ((pending - current) * movement < 0 || (movement != 0 && qAbs(pending - current) > qAbs(movement) * speedLimit))
    {
        pending = current;
    }
    return qBound(minimum, pending + movement, maximum);
}

qreal ElaScrollAnimation::_getDuration(qreal distance)
{
    return std::log1p(qAbs(distance) * _velocityPerPixel / _minimumVelocity) / _velocityPerPixel;
}

qreal ElaScrollAnimation::_getPosition(qreal start, qreal target, qreal elapsed)
{
    const qreal distance = target - start;
    if (elapsed >= _getDuration(distance))
    {
        return target;
    }
    const qreal velocity = distance * _velocityPerPixel + std::copysign(_minimumVelocity, distance);
    return start - velocity * std::expm1(-_velocityPerPixel * elapsed) / _velocityPerPixel;
}
