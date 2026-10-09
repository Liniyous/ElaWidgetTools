#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELACALENDARPICKER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELACALENDARPICKER_H_

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
#include <QDate>
#include <QPushButton>
class ElaCalendarPickerPrivate;
class ELA_EXPORT ElaCalendarPicker : public QPushButton
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaCalendarPicker)
    ELA_PROPERTY_Q_H(int, BorderRadius)

public:
    explicit ElaCalendarPicker(QWidget* parent = nullptr);
    ~ElaCalendarPicker() override;

    void setSelectedDate(QDate selectedDate);
    Q_REQUIRED_RESULT QDate getSelectedDate() const;

Q_SIGNALS:
    Q_SIGNAL void selectedDateChanged(QDate date);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELACALENDARPICKER_H_
