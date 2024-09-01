#ifndef T_DxgiWidget_H
#define T_DxgiWidget_H

#include <QWidget>
#include "T_AppConfig.h"
#include "T_ElaScreen.h"
class DxgiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DxgiWidget(QWidget* parent = nullptr);
    DxgiWidget(QWidget* parent = nullptr,T_ElaScreen* _elaScreenPage= nullptr);
    ElaScrollPageArea *createPageArea(QString text, QString widgetType);
    ~DxgiWidget();
signals:
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
    ElaComboBox* _outputComboBox; //采集屏幕对象
    T_ElaScreen* t_ElaScreen;
};

#endif // T_DxgiWidget_H
