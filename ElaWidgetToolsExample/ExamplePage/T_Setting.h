#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_SETTING_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_SETTING_H_

#include "T_BasePage.h"

class ElaToggleSwitch;
class ElaComboBox;
class T_Setting : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Setting(QWidget* parent = nullptr);
    ~T_Setting() override;

private:
    ElaComboBox* _themeComboBox{nullptr};
    ElaToggleSwitch* _logSwitchButton{nullptr};
    ElaToggleSwitch* _userCardSwitchButton{nullptr};
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_SETTING_H_
