#ifndef T_BASECOMPONENTS_H
#define T_BASECOMPONENTS_H

#include <ElaScrollPage.h>

class ElaToggleSwitch;
class ElaToggleButton;
class ElaComboBox;
class ElaMultiSelectComboBox;
class ElaMessageButton;
class ElaCheckBox;
class ElaSpinBox;
class ElaSlider;
class ElaRadioButton;
class ElaProgressBar;
class T_BaseComponents : public ElaScrollPage
{
    Q_OBJECT
public:
    T_BaseComponents(QWidget* parent = nullptr);
    ~T_BaseComponents();

private:
    ElaToggleSwitch* _toggleSwitch{nullptr};
    ElaToggleButton* _toggleButton{nullptr};
    ElaComboBox* _comboBox{nullptr};
    ElaMultiSelectComboBox* _multiSelectComboBox{nullptr};
    ElaMessageButton* _messageButton{nullptr};
    ElaMessageButton* _infoMessageButton{nullptr};
    ElaMessageButton* _warningMessageButton{nullptr};
    ElaMessageButton* _errorMessageButton{nullptr};
    ElaCheckBox* _checkBox{nullptr};
    ElaSpinBox* _spinBox{nullptr};
    ElaSlider* _slider{nullptr};
    ElaRadioButton* _radioButton{nullptr};
    ElaProgressBar* _progressBar{nullptr};
};

#endif // T_BASECOMPONENTS_H
