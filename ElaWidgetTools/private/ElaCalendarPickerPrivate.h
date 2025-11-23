#ifndef ELACALENDARPICKERPRIVATE_H
#define ELACALENDARPICKERPRIVATE_H

#include <QObject>

#include "ElaDef.h"
class ElaCalendar;
class ElaCalendarPicker;
class ElaCalendarPickerContainer;
class ElaCalendarPickerPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaCalendarPicker)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
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

#endif // ELACALENDARPICKERPRIVATE_H
