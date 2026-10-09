#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELACALENDAR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELACALENDAR_H_

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
#include <QDate>
#include <QWidget>
class ElaCalendarPrivate;
class ELA_EXPORT ElaCalendar : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaCalendar)
    ELA_PROPERTY_Q_H(int, BorderRaiuds)
public:
    explicit ElaCalendar(QWidget* parent = nullptr);
    ~ElaCalendar() override;

    void setSelectedDate(QDate selectedDate);
    Q_REQUIRED_RESULT QDate getSelectedDate() const;
    void setMinimumDate(QDate minimudate);
    Q_REQUIRED_RESULT QDate getMinimumDate() const;
    void setMaximumDate(QDate maximumDate);
    Q_REQUIRED_RESULT QDate getMaximumDate() const;

Q_SIGNALS:
    Q_SIGNAL void pSelectedDateChanged();
    Q_SIGNAL void pMinimumDateChanged();
    Q_SIGNAL void pMaximumDateChanged();
    Q_SIGNAL void clicked(QDate date);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELACALENDAR_H_
