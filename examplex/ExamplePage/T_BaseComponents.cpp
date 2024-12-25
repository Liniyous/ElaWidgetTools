#include "T_BaseComponents.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaTheme.h"
#include "ElaPushButton.h"
#include "ElaCheckBox.h"
#include "ElaComboBox.h"
#include "ElaMessageButton.h"
#include "ElaMultiSelectComboBox.h"
#include "ElaPlainTextEdit.h"
#include "ElaProgressBar.h"
#include "ElaRadioButton.h"
#include "ElaScrollPageArea.h"
#include "ElaSlider.h"
#include "ElaSpinBox.h"
#include "ElaText.h"
#include "ElaToggleButton.h"
#include "ElaToggleSwitch.h"
T_BaseComponents::T_BaseComponents(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaBaseComponents");

    // 顶部元素
    createCustomWidget("一些常用的基础组件被放置于此，可在此界面体验其效果并按需添加进项目中");

    _toggleSwitch = new ElaToggleSwitch(this);
    ElaScrollPageArea* toggleSwitchArea = new ElaScrollPageArea(this);
    toggleSwitchArea->setFixedHeight(75);
    QHBoxLayout* toggleSwitchLayout = new QHBoxLayout(toggleSwitchArea);
    toggleSwitchLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* toggleSwitchText = new ElaText("ElaToggleSwitch", this);
    toggleSwitchText->setTextPixelSize(15);
    toggleSwitchLayout->addWidget(toggleSwitchText);
    toggleSwitchLayout->addWidget(_toggleSwitch);
    toggleSwitchLayout->addStretch();
    ElaToggleSwitch* toggleSwitchDisableSwitch = new ElaToggleSwitch(this);
    ElaText* toggleSwitchDisableText = new ElaText("禁用", this);
    toggleSwitchDisableText->setTextPixelSize(15);
    connect(toggleSwitchDisableSwitch, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        _toggleSwitch->setDisabled(checked);
    });
    toggleSwitchLayout->addWidget(toggleSwitchDisableSwitch);
    toggleSwitchLayout->addWidget(toggleSwitchDisableText);
    toggleSwitchLayout->addSpacing(10);

    _toggleButton = new ElaToggleButton("ToggleButton", this);
    _toggleButton->setFixedWidth(120);
    ElaScrollPageArea* toggleButtonArea = new ElaScrollPageArea(this);
    toggleButtonArea->setFixedHeight(75);
    QHBoxLayout* toggleButtonLayout = new QHBoxLayout(toggleButtonArea);
    toggleButtonLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* toggleButtonText = new ElaText("ElaToggleButton", this);
    toggleButtonText->setTextPixelSize(15);
    toggleButtonLayout->addWidget(toggleButtonText);
    toggleButtonLayout->addWidget(_toggleButton);
    toggleButtonLayout->addStretch();
    ElaToggleSwitch* toggleButtonDisableSwitch = new ElaToggleSwitch(this);
    ElaText* toggleButtonDisableText = new ElaText("禁用", this);
    toggleButtonDisableText->setTextPixelSize(15);
    connect(toggleButtonDisableSwitch, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        _toggleButton->setDisabled(checked);
    });
    toggleButtonLayout->addWidget(toggleButtonDisableSwitch);
    toggleButtonLayout->addWidget(toggleButtonDisableText);
    toggleButtonLayout->addSpacing(10);

    _pushButton = new ElaPushButton("pushButton", this);
    _pushButton->setFixedWidth(120);
    _pushButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));
    _pushButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    _pushButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    _pushButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    _pushButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    _pushButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    _pushButton->setLightTextColor(Qt::white);
    _pushButton->setDarkTextColor(Qt::black);
    ElaScrollPageArea* pushButtonArea = new ElaScrollPageArea(this);
    pushButtonArea->setFixedHeight(75);
    QHBoxLayout* pushButtonLayout = new QHBoxLayout(pushButtonArea);
    pushButtonLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* pushButtonText = new ElaText("ElaPushButton", this);
    pushButtonText->setTextPixelSize(15);
    pushButtonLayout->addWidget(pushButtonText);
    pushButtonLayout->addWidget(_pushButton);
    pushButtonLayout->addStretch();
    ElaToggleSwitch* pushButtonDisableSwitch = new ElaToggleSwitch(this);
    ElaText* pushButtonDisableText = new ElaText("禁用", this);
    pushButtonDisableText->setTextPixelSize(15);
    connect(pushButtonDisableSwitch, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        _pushButton->setDisabled(checked);
    });
    pushButtonLayout->addWidget(pushButtonDisableSwitch);
    pushButtonLayout->addWidget(pushButtonDisableText);
    pushButtonLayout->addSpacing(10);

    _comboBox = new ElaComboBox(this);
    QStringList comboList{
        "我愿投身前途未卜的群星",
        "潜行 步伐小心翼翼",
        "不留游走痕迹",
        "如同一簇幽灵",
        "所谓 道德加上伦理",
        "抱歉只能律己"};
    _comboBox->addItems(comboList);
    ElaScrollPageArea* comboBoxArea = new ElaScrollPageArea(this);
    comboBoxArea->setFixedHeight(75);
    QHBoxLayout* comboBoxLayout = new QHBoxLayout(comboBoxArea);
    comboBoxLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* comboBoxText = new ElaText("ElaComboBox", this);
    comboBoxText->setTextPixelSize(15);
    comboBoxLayout->addWidget(comboBoxText);
    comboBoxLayout->addWidget(_comboBox);
    comboBoxLayout->addStretch();
    ElaToggleSwitch* comboBoxDisableSwitch = new ElaToggleSwitch(this);
    ElaText* comboBoxDisableText = new ElaText("禁用", this);
    comboBoxDisableText->setTextPixelSize(15);
    connect(comboBoxDisableSwitch, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        _comboBox->setDisabled(checked);
    });
    comboBoxLayout->addWidget(comboBoxDisableSwitch);
    comboBoxLayout->addWidget(comboBoxDisableText);
    comboBoxLayout->addSpacing(10);

    _multiSelectComboBox = new ElaMultiSelectComboBox(this);
    QStringList multiComboList{"执念的鱼", "提着灯闯过远洋的甄选", "继续下潜", "无需誓言", "我的心像自沉的旧母舰", "没入深渊", "我曾凝望曾是航向的日出"};
    QStringList multiSelectComboList{"执念的鱼", "提着灯闯过远洋的甄选", "无需誓言", "我的心像自沉的旧母舰"};
    _multiSelectComboBox->addItems(multiComboList);
    _multiSelectComboBox->setCurrentSelection(multiSelectComboList);
    ElaScrollPageArea* multiSelectComboBoxArea = new ElaScrollPageArea(this);
    multiSelectComboBoxArea->setFixedHeight(75);
    QHBoxLayout* multiSelectComboBoxLayout = new QHBoxLayout(multiSelectComboBoxArea);
    multiSelectComboBoxLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* multiSelectComboBoxText = new ElaText("ElaMutilSelectComboBox", this);
    multiSelectComboBoxText->setTextPixelSize(15);
    multiSelectComboBoxLayout->addWidget(multiSelectComboBoxText);
    multiSelectComboBoxLayout->addWidget(_multiSelectComboBox);
    multiSelectComboBoxLayout->addStretch();
    ElaToggleSwitch* multiSelectComboBoxDisableSwitch = new ElaToggleSwitch(this);
    ElaText* multiSelectComboBoxDisableText = new ElaText("禁用", this);
    multiSelectComboBoxDisableText->setTextPixelSize(15);
    connect(multiSelectComboBoxDisableSwitch, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        _multiSelectComboBox->setDisabled(checked);
    });
    multiSelectComboBoxLayout->addWidget(multiSelectComboBoxDisableSwitch);
    multiSelectComboBoxLayout->addWidget(multiSelectComboBoxDisableText);
    multiSelectComboBoxLayout->addSpacing(10);

    _messageButton = new ElaMessageButton("Success", this);
    _messageButton->setBarTitle("Success");
    _messageButton->setBarText("登陆成功!");

    _infoMessageButton = new ElaMessageButton("Info", this);
    _infoMessageButton->setBarTitle("Information");
    _infoMessageButton->setBarText("点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");
    _infoMessageButton->setMessageMode(ElaMessageBarType::Information);
    _infoMessageButton->setPositionPolicy(ElaMessageBarType::TopLeft);

    _warningMessageButton = new ElaMessageButton("Warning", this);
    _warningMessageButton->setBarTitle("Warning");
    _warningMessageButton->setBarText("点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明 点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明 点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");
    _warningMessageButton->setMessageMode(ElaMessageBarType::Warning);
    _warningMessageButton->setPositionPolicy(ElaMessageBarType::BottomLeft);

    _errorMessageButton = new ElaMessageButton("Error", this);
    _errorMessageButton->setBarTitle("Error");
    _errorMessageButton->setBarText("网络连接失败，请稍后再试。");
    _errorMessageButton->setMessageMode(ElaMessageBarType::Error);
    _errorMessageButton->setPositionPolicy(ElaMessageBarType::BottomRight);

    ElaScrollPageArea* messageButtonArea = new ElaScrollPageArea(this);
    messageButtonArea->setFixedHeight(75);
    QHBoxLayout* messageButtonLayout = new QHBoxLayout(messageButtonArea);
    messageButtonLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* messageButtonText = new ElaText("ElaMessageButton", this);
    messageButtonText->setTextPixelSize(15);
    messageButtonLayout->addWidget(messageButtonText);
    messageButtonLayout->addWidget(_messageButton);
    messageButtonLayout->addWidget(_infoMessageButton);
    messageButtonLayout->addWidget(_warningMessageButton);
    messageButtonLayout->addWidget(_errorMessageButton);
    messageButtonLayout->addStretch();
    ElaToggleSwitch* messageButtonDisableSwitch = new ElaToggleSwitch(this);
    ElaText* messageButtonDisableText = new ElaText("禁用", this);
    messageButtonDisableText->setTextPixelSize(15);
    connect(messageButtonDisableSwitch, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        _messageButton->setDisabled(checked);
        _infoMessageButton->setDisabled(checked);
        _warningMessageButton->setDisabled(checked);
        _errorMessageButton->setDisabled(checked);
    });
    messageButtonLayout->addWidget(messageButtonDisableSwitch);
    messageButtonLayout->addWidget(messageButtonDisableText);
    messageButtonLayout->addSpacing(10);

    _checkBox = new ElaCheckBox("CheckBox", this);
    ElaScrollPageArea* checkBoxArea = new ElaScrollPageArea(this);
    checkBoxArea->setFixedHeight(75);
    QHBoxLayout* checkBoxLayout = new QHBoxLayout(checkBoxArea);
    checkBoxLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* checkBoxText = new ElaText("ElacheckBox", this);
    checkBoxText->setTextPixelSize(15);
    checkBoxLayout->addWidget(checkBoxText);
    checkBoxLayout->addWidget(_checkBox);
    checkBoxLayout->addStretch();
    ElaToggleSwitch* checkBoxDisableSwitch = new ElaToggleSwitch(this);
    ElaText* checkBoxDisableText = new ElaText("禁用", this);
    checkBoxDisableText->setTextPixelSize(15);
    connect(checkBoxDisableSwitch, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        _checkBox->setDisabled(checked);
    });
    checkBoxLayout->addWidget(checkBoxDisableSwitch);
    checkBoxLayout->addWidget(checkBoxDisableText);
    checkBoxLayout->addSpacing(10);

    _spinBox = new ElaSpinBox(this);
    ElaScrollPageArea* spinBoxArea = new ElaScrollPageArea(this);
    spinBoxArea->setFixedHeight(75);
    QHBoxLayout* spinBoxLayout = new QHBoxLayout(spinBoxArea);
    spinBoxLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* spinBoxText = new ElaText("ElaSpinBox", this);
    spinBoxText->setTextPixelSize(15);
    spinBoxLayout->addWidget(spinBoxText);
    spinBoxLayout->addWidget(_spinBox);
    spinBoxLayout->addStretch();

    _slider = new ElaSlider(this);
    ElaScrollPageArea* sliderArea = new ElaScrollPageArea(this);
    sliderArea->setFixedHeight(75);
    QHBoxLayout* sliderLayout = new QHBoxLayout(sliderArea);
    sliderLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* sliderText = new ElaText("ElaSlider", this);
    sliderText->setTextPixelSize(15);
    sliderLayout->addWidget(sliderText);
    sliderLayout->addWidget(_slider);
    sliderLayout->addStretch();

    _radioButton = new ElaRadioButton("RadioButton", this);
    ElaScrollPageArea* radioButtonArea = new ElaScrollPageArea(this);
    radioButtonArea->setFixedHeight(75);
    QHBoxLayout* radioButtonLayout = new QHBoxLayout(radioButtonArea);
    radioButtonLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* radioButtonText = new ElaText("ElaRadioButton", this);
    radioButtonText->setTextPixelSize(15);
    radioButtonLayout->addWidget(radioButtonText);
    radioButtonLayout->addWidget(_radioButton);
    radioButtonLayout->addStretch();

    _progressBar = new ElaProgressBar(this);
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(0);
    ElaScrollPageArea* progressBarArea = new ElaScrollPageArea(this);
    progressBarArea->setFixedHeight(75);
    QHBoxLayout* progressBarLayout = new QHBoxLayout(progressBarArea);
    progressBarLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* progressBarText = new ElaText("ElaProgressBar", this);
    progressBarText->setTextPixelSize(15);
    progressBarLayout->addWidget(progressBarText);
    progressBarLayout->addWidget(_progressBar);
    progressBarLayout->addStretch();

    ElaPlainTextEdit* edit = new ElaPlainTextEdit(this);
    edit->document()->setDocumentMargin(10);
    edit->setPlainText("这是一个ElaPlainTextEdit  暂时放在这里");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaBaseComponents");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->addWidget(toggleSwitchArea);
    centerLayout->addWidget(pushButtonArea);
    centerLayout->addWidget(toggleButtonArea);
    centerLayout->addWidget(comboBoxArea);
    centerLayout->addWidget(multiSelectComboBoxArea);
    centerLayout->addWidget(messageButtonArea);
    centerLayout->addWidget(checkBoxArea);
    centerLayout->addWidget(spinBoxArea);
    centerLayout->addWidget(sliderArea);
    centerLayout->addWidget(radioButtonArea);
    centerLayout->addWidget(progressBarArea);
    centerLayout->addWidget(edit);
    centerLayout->addStretch();
    centerLayout->setContentsMargins(0, 0, 20, 20);
    addCentralWidget(centralWidget, true, true, 0);

    ElaText* homeStack1 = new ElaText("HomeStack1", this);
    QFont font = homeStack1->font();
    font.setPixelSize(32);
    homeStack1->setFont(font);
    homeStack1->setAlignment(Qt::AlignCenter);
    homeStack1->setWindowTitle("HomeStack1");
    addCentralWidget(homeStack1);
    ElaText* homeStack2 = new ElaText("HomeStack2", this);
    homeStack2->setFont(font);
    homeStack2->setAlignment(Qt::AlignCenter);
    homeStack2->setWindowTitle("HomeStack2");
    addCentralWidget(homeStack2);
}

T_BaseComponents::~T_BaseComponents()
{
}

void T_BaseComponents::mouseReleaseEvent(QMouseEvent* event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
    {
        //ElaMessageBar::success(ElaMessageBarType::TopRight, "Success", "Never Close Your Eyes", 2500);
        //ElaMessageBar::success(ElaMessageBarType::TopRight, "Success", "Never Close Your Eyes", 1500);
        break;
    }
    case Qt::BackButton:
    {
        this->navigation(0);
        break;
    }
    case Qt::ForwardButton:
    {
        this->navigation(1);
        break;
    }
    case Qt::MiddleButton:
    {
        this->navigation(2);
        break;
    }
    default:
    {
        break;
    }
    }
    ElaScrollPage::mouseReleaseEvent(event);
}
