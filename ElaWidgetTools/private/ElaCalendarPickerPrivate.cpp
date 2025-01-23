#include "ElaCalendarPickerPrivate.h"

#include <QDate>
#include <QPropertyAnimation>

#include "ElaCalendar.h"
#include "ElaCalendarPicker.h"
#include "ElaCalendarPickerContainer.h"
ElaCalendarPickerPrivate::ElaCalendarPickerPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaCalendarPickerPrivate::~ElaCalendarPickerPrivate()
{
}

void ElaCalendarPickerPrivate::onCalendarPickerClicked()
{
    Q_Q(ElaCalendarPicker);
    QPoint endPoint(q->mapToGlobal(QPoint((q->width() - _calendarPickerContainer->width()) / 2, q->height() + 5)));
    QPropertyAnimation* showAnimation = new QPropertyAnimation(_calendarPickerContainer, "pos");
    showAnimation->setEasingCurve(QEasingCurve::OutCubic);
    showAnimation->setDuration(250);
    showAnimation->setStartValue(QPoint(endPoint.x(), endPoint.y() - 10));
    showAnimation->setEndValue(endPoint);
    _calendarPickerContainer->show();
    showAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaCalendarPickerPrivate::onCalendarSelectedDateChanged()
{
    Q_Q(ElaCalendarPicker);
    Q_EMIT q->selectedDateChanged(_calendar->getSelectedDate());
    if (_calendarPickerContainer->isVisible())
    {
        _calendarPickerContainer->hide();
    }
}
