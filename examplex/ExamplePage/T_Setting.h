#ifndef T_SETTING_H
#define T_SETTING_H

#include "T_BasePage.h"
#include "../Function/database.h"

class ElaRadioButton;
class ElaToggleSwitch;
class ElaComboBox;
class T_Setting : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Setting(QWidget* parent = nullptr);
    ~T_Setting();

private:
    ElaComboBox* _themeComboBox{nullptr};
    ElaToggleSwitch* _logSwitchButton{nullptr};
    ElaRadioButton* _minimumButton{nullptr};
    ElaRadioButton* _compactButton{nullptr};
    ElaRadioButton* _maximumButton{nullptr};
    ElaRadioButton* _autoButton{nullptr};
    ElaToggleSwitch* _transSwitchButton{nullptr};
    ElaToggleSwitch* _userSwitchButton{nullptr};

    ElaRadioButton* _dwmblueButton{nullptr};
    ElaRadioButton* _acrylicButton{nullptr};
    ElaRadioButton* _micaButton{nullptr};
    ElaRadioButton* _altButton{nullptr};
    ElaRadioButton* _noneButton{nullptr};

    bool initing=false;
    void initConf();
};

#endif // T_SETTING_H
