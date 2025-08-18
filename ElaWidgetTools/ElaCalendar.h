#ifndef ELACALENDAR_H
#define ELACALENDAR_H

#include <QWidget>

#include "ElaProperty.h"
class ElaCalendarPrivate;
class ELA_EXPORT ElaCalendar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaCalendar)
    Q_PROPERTY_CREATE_Q_H(int, BorderRaiuds)
    Q_PROPERTY_CREATE_Q_H(QDate, SelectedDate)
    Q_PROPERTY_CREATE_Q_H(QDate, MinimumDate)
    Q_PROPERTY_CREATE_Q_H(QDate, MaximumDate)
public:
    explicit ElaCalendar(QWidget* parent = nullptr);
    ~ElaCalendar();
Q_SIGNALS:
    Q_SIGNAL void clicked(QDate date);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELACALENDAR_H
