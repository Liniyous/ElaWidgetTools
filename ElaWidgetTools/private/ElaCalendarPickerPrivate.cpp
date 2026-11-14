#include "ElaCalendarPickerPrivate.h"

#include "ElaCalendar.h"
#include "ElaCalendarPicker.h"
#include "ElaCalendarPickerContainer.h"
#include "ElaControlAnimation.h"
#include "ElaTheme.h"

#include <QDate>
#include <QEvent>
#include <QFocusEvent>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QScreen>
#include <QTimer>
#include <QVariantAnimation>

ElaCalendarPickerPrivate::ElaCalendarPickerPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaCalendarPickerPrivate::~ElaCalendarPickerPrivate()
{
}

void ElaCalendarPickerPrivate::onCalendarPickerClicked()
{
    if (_calendarPickerContainer->isVisible())
    {
        _closePopup();
    }
    else
    {
        _popupDelayTimer->start();
    }
}

void ElaCalendarPickerPrivate::onPopupDelayTimeout()
{
    Q_Q(ElaCalendarPicker);
    if (!q->isVisible() || !q->isEnabled() || _calendarPickerContainer->isVisible())
    {
        return;
    }
    const QRect buttonRect(q->mapToGlobal(QPoint()), q->size());
    QScreen* screen = QGuiApplication::screenAt(buttonRect.center());
    if (!screen)
    {
        screen = QGuiApplication::primaryScreen();
    }
    if (!screen)
    {
        return;
    }
    const QRect available = screen->availableGeometry().adjusted(4, 4, -4, -4);
    const QSize size = _calendarPickerContainer->size();
    const QMargins margins = eTheme->getShadowMargins();
    int x = buttonRect.center().x() - size.width() / 2;
    int y = buttonRect.bottom() + 5 - margins.top();
    _popupOffset = -10;
    if (y + size.height() > available.bottom() + 1)
    {
        y = buttonRect.top() - 4 - size.height() + margins.bottom();
        _popupOffset = 10;
    }
    x = qBound(available.left(), x, qMax(available.left(), available.right() - size.width() + 1));
    y = qBound(available.top(), y, qMax(available.top(), available.bottom() - size.height() + 1));
    _popupPosition = QPoint(x, y);
    _popupAnimation->stop();
    _isClosing = false;
    const bool isAnimation = ElaControlAnimation::getIsAnimationEnabled(q);
    _calendarPickerContainer->move(_popupPosition + QPoint(0, isAnimation ? _popupOffset : 0));
    _calendarPickerContainer->setWindowOpacity(isAnimation ? 0 : 1);
    _calendarPickerContainer->show();
    _calendar->setFocus(Qt::PopupFocusReason);
    if (isAnimation)
    {
        _popupAnimation->setDuration(250);
        _popupAnimation->start();
    }
}

void ElaCalendarPickerPrivate::onCalendarSelectedDateChanged()
{
    Q_Q(ElaCalendarPicker);
    const QDate date = _calendar->getSelectedDate();
    _closePopup();
    q->update();
    Q_EMIT q->selectedDateChanged(date);
}

bool ElaCalendarPickerPrivate::eventFilter(QObject* watched, QEvent* event)
{
    Q_Q(ElaCalendarPicker);
    if (watched == q)
    {
        const bool isUnavailable = event->type() == QEvent::Hide || (event->type() == QEvent::EnabledChange && !q->isEnabled());
        const bool isFocusLost = event->type() == QEvent::FocusOut && static_cast<QFocusEvent*>(event)->reason() != Qt::PopupFocusReason;
        if (isUnavailable || isFocusLost || (event->type() == QEvent::WindowDeactivate && !_calendarPickerContainer->isVisible()))
        {
            _popupDelayTimer->stop();
            if (isUnavailable)
            {
                _calendarPickerContainer->hide();
            }
        }
        if (event->type() == QEvent::KeyPress)
        {
            const auto keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Escape)
            {
                _popupDelayTimer->stop();
                _closePopup();
                return true;
            }
            if (!keyEvent->isAutoRepeat() && q->isEnabled() && (keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_F4 || keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter))
            {
                onCalendarPickerClicked();
                return true;
            }
        }
    }
    else if (watched == _calendarPickerContainer && event->type() == QEvent::Hide)
    {
        _popupDelayTimer->stop();
        _popupAnimation->stop();
        _isClosing = false;
        _calendarPickerContainer->setWindowOpacity(1);
        _controlAnimation->release();
        q->update();
    }
    return QObject::eventFilter(watched, event);
}

void ElaCalendarPickerPrivate::_closePopup()
{
    Q_Q(ElaCalendarPicker);
    _popupDelayTimer->stop();
    if (!_calendarPickerContainer->isVisible() || _isClosing)
    {
        return;
    }
    _popupAnimation->stop();
    if (!ElaControlAnimation::getIsAnimationEnabled(q))
    {
        _calendarPickerContainer->hide();
        return;
    }
    _isClosing = true;
    _popupStartOpacity = _calendarPickerContainer->windowOpacity();
    _popupAnimation->setDuration(83);
    _popupAnimation->start();
}
