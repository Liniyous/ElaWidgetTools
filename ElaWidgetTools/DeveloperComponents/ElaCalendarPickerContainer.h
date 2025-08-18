#ifndef ELACALENDARPICKERCONTAINER_H
#define ELACALENDARPICKERCONTAINER_H

#include <QWidget>

#include "ElaDef.h"
class ElaCalendarPickerContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ElaCalendarPickerContainer(QWidget* parent = nullptr);
    ~ElaCalendarPickerContainer();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELACALENDARPICKERCONTAINER_H
