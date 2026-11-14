#include "ElaControlAnimation.h"

#include "ElaColorAnimation.h"

#include <QApplication>
#include <QEasingCurve>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStyle>
#include <QVariantAnimation>
#include <QWidget>

ElaControlAnimation::ElaControlAnimation(QWidget* widget)
    : QObject(widget)
{
    _widget = widget;
    widget->installEventFilter(this);
    _chevronAnimation = new QVariantAnimation(this);
    connect(_chevronAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
        const qreal progress = value.toReal();
        if (_isPressed)
        {
            QEasingCurve easing(QEasingCurve::BezierSpline);
            easing.addCubicBezierSegment(QPointF(0.167, 0.167), QPointF(0.65, 1), QPointF(1, 1));
            _chevronOffset = _releaseOffset + (1.875 - _releaseOffset) * easing.valueForProgress(progress);
        }
        else
        {
            // WinUI 小箭头以十二像素显示 四十八像素源图中的位移按比例换算
            const qreal turn = 5.0 / 19.0;
            QEasingCurve easing(QEasingCurve::BezierSpline);
            if (progress < turn)
            {
                easing.addCubicBezierSegment(QPointF(0.55, 0), QPointF(0.75, 1), QPointF(1, 1));
                _chevronOffset = _releaseOffset + (-0.75 - _releaseOffset) * easing.valueForProgress(progress / turn);
            }
            else
            {
                easing.addCubicBezierSegment(QPointF(0.35, 0), QPointF(0, 1), QPointF(1, 1));
                _chevronOffset = -0.75 * (1 - easing.valueForProgress((progress - turn) / (1 - turn)));
            }
        }
        if (_widget)
        {
            _widget->update();
        }
    });
}

QColor ElaControlAnimation::getBackgroundColor(const QColor& target)
{
    return ElaColorAnimation::getColor(_widget, target);
}

qreal ElaControlAnimation::getChevronOffset() const
{
    return _chevronOffset;
}

bool ElaControlAnimation::getIsPressed() const
{
    return _isPressed;
}

void ElaControlAnimation::release()
{
    _setPressed(false);
}

bool ElaControlAnimation::getIsAnimationEnabled(const QWidget* widget)
{
    return ElaColorAnimation::getIsAnimationEnabled(widget);
}

bool ElaControlAnimation::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == _widget)
    {
        switch (event->type())
        {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonDblClick:
        {
            if (static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton && _widget->isEnabled())
            {
                _setPressed(true);
            }
            break;
        }
        case QEvent::KeyPress:
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (!keyEvent->isAutoRepeat() && (keyEvent->key() == Qt::Key_Space || keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter))
            {
                _setPressed(true);
            }
            break;
        }
        case QEvent::MouseButtonRelease:
        case QEvent::KeyRelease:
        case QEvent::FocusOut:
        case QEvent::Leave:
        {
            release();
            break;
        }
        case QEvent::EnabledChange:
        case QEvent::Hide:
        {
            _isPressed = false;
            _chevronAnimation->stop();
            _chevronOffset = 0;
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

void ElaControlAnimation::_setPressed(bool isPressed)
{
    if (_isPressed == isPressed)
    {
        return;
    }
    _isPressed = isPressed;
    _chevronAnimation->stop();
    _releaseOffset = _chevronOffset;
    if (!getIsAnimationEnabled(_widget))
    {
        _chevronOffset = 0;
    }
    else
    {
        _chevronAnimation->setDuration(isPressed ? 150 : 300);
        _chevronAnimation->setStartValue(0.0);
        _chevronAnimation->setEndValue(1.0);
        _chevronAnimation->start();
    }
    if (_widget)
    {
        _widget->update();
    }
}
