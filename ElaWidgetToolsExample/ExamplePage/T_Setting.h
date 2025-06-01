#ifndef T_SETTING_H
#define T_SETTING_H

#include "T_BasePage.h"
class ElaRadioButton;
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
    ElaRadioButton* _normalButton{nullptr};
    ElaRadioButton* _elaMicaButton{nullptr};
#ifdef Q_OS_WIN
    ElaRadioButton* _micaButton{nullptr};
    ElaRadioButton* _micaAltButton{nullptr};
    ElaRadioButton* _acrylicButton{nullptr};
    ElaRadioButton* _dwmBlurnormalButton{nullptr};
#endif
    ElaToggleSwitch* _logSwitchButton{nullptr};
    ElaRadioButton* _minimumButton{nullptr};
    ElaRadioButton* _compactButton{nullptr};
    ElaRadioButton* _maximumButton{nullptr};
    ElaRadioButton* _autoButton{nullptr};

    ElaRadioButton* _noneButton{nullptr};
    ElaRadioButton* _popupButton{nullptr};
    ElaRadioButton* _scaleButton{nullptr};
    ElaRadioButton* _flipButton{nullptr};
};

#endif // T_SETTING_H
