#include "ElaInputFocusAnimation.h"

#include "ElaColorAnimation.h"

#include <QFocusEvent>
#include <QWidget>

ElaInputFocusAnimation::ElaInputFocusAnimation(QObject* style, QWidget* widget)
    : QPropertyAnimation(style, "pExpandMarkProgress", widget)
{
    _widget = widget;
    setDuration(180);
    setEasingCurve(QEasingCurve::OutCubic);
    widget->installEventFilter(this);
    _syncFocusProxy();
    connect(this, &QVariantAnimation::valueChanged, widget, [widget]() {
        widget->update();
    });
}

bool ElaInputFocusAnimation::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == _widget)
    {
        switch (event->type())
        {
        case QEvent::Polish:
        case QEvent::ChildAdded:
        {
            _syncFocusProxy();
            break;
        }
        case QEvent::Enter:
        case QEvent::Leave:
        {
            _widget->update();
            break;
        }
        case QEvent::FocusIn:
        case QEvent::FocusOut:
        {
            if (static_cast<QFocusEvent*>(event)->reason() != Qt::PopupFocusReason)
            {
                _animateFocus(_widget->hasFocus());
                _widget->update();
            }
            break;
        }
        case QEvent::EnabledChange:
        case QEvent::Hide:
        {
            if (!_widget->isEnabled() || event->type() == QEvent::Hide)
            {
                stop();
                if (targetObject())
                {
                    targetObject()->setProperty("pExpandMarkProgress", 0.0);
                }
                _widget->update();
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }
    else if (watched && watched == _focusProxy)
    {
        if (event->type() == QEvent::FocusIn || event->type() == QEvent::FocusOut)
        {
            if (static_cast<QFocusEvent*>(event)->reason() != Qt::PopupFocusReason)
            {
                _animateFocus(_widget->hasFocus());
                _widget->update();
            }
        }
    }
    return QPropertyAnimation::eventFilter(watched, event);
}

void ElaInputFocusAnimation::_syncFocusProxy()
{
    if (!_widget)
    {
        return;
    }
    QWidget* proxy = _widget->focusProxy();
    if (_focusProxy == proxy)
    {
        return;
    }
    if (_focusProxy)
    {
        _focusProxy->removeEventFilter(this);
    }
    _focusProxy = proxy;
    if (_focusProxy && _focusProxy != _widget)
    {
        _focusProxy->installEventFilter(this);
    }
}

void ElaInputFocusAnimation::_animateFocus(bool isFocused)
{
    if (!targetObject())
    {
        return;
    }
    stop();
    const qreal targetProgress = isFocused ? 1.0 : 0.0;
    const qreal currentProgress = targetObject()->property("pExpandMarkProgress").toReal();
    if (!ElaColorAnimation::getIsAnimationEnabled(_widget) || qFuzzyCompare(currentProgress + 1, targetProgress + 1))
    {
        targetObject()->setProperty("pExpandMarkProgress", targetProgress);
        _widget->update();
        return;
    }
    setDuration(isFocused ? 180 : 150);
    setEasingCurve(QEasingCurve::OutCubic);
    setStartValue(currentProgress);
    setEndValue(targetProgress);
    start();
}
