#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_ELASCREEN_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_ELASCREEN_H_

#include "T_BasePage.h"

class ElaComboBox;
class ElaImageCard;
class ElaToggleButton;
class T_ElaScreen : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_ElaScreen(QWidget* parent = nullptr);
    ~T_ElaScreen() override;

private:
    ElaImageCard* _captureImageCard{nullptr};       // 本地采集画面的预览区域
    ElaComboBox* _graphicsComboBox{nullptr};        // 当前图形适配器
    ElaComboBox* _outputComboBox{nullptr};          // 当前显示输出设备
    ElaToggleButton* _captureToggleButton{nullptr}; // 屏幕采集开关
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_ELASCREEN_H_
