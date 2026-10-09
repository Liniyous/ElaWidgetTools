#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACALENDARPICKERPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACALENDARPICKERPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaCalendar;
class ElaCalendarPicker;
class ElaCalendarPickerContainer;
class ElaCalendarPickerPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaCalendarPicker)
    ELA_PROPERTY_D(int, BorderRadius)
public:
    explicit ElaCalendarPickerPrivate(QObject* parent = nullptr);
    ~ElaCalendarPickerPrivate() override;
    Q_SLOT void onCalendarPickerClicked();
    Q_SLOT void onCalendarSelectedDateChanged();

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaCalendar* _calendar{nullptr};
    ElaCalendarPickerContainer* _calendarPickerContainer{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACALENDARPICKERPRIVATE_H_

