#ifndef T_POPUP_H
#define T_POPUP_H

#include "T_BasePage.h"

class ElaToolButton;
class ElaColorDialog;
class ElaCalendar;
class ElaCalendarPicker;
class ElaKeyBinder;
class T_Popup : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Popup(QWidget* parent = nullptr);
    ~T_Popup();

private:
    ElaToolButton* _toolButton{nullptr};
    ElaColorDialog* _colorDialog{nullptr};
    ElaCalendar* _calendar{nullptr};
    ElaCalendarPicker* _calendarPicker{nullptr};
    ElaKeyBinder* _keyBinder{nullptr};
};

#endif// T_POPUP_H
