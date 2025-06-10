#include "T_BaseComponents.h"

#include "ElaCheckBox.h"
#include "ElaComboBox.h"
#include "ElaMessageButton.h"
#include "ElaMultiSelectComboBox.h"
#include "ElaPlainTextEdit.h"
#include "ElaProgressBar.h"
#include "ElaProgressRing.h"
#include "ElaRadioButton.h"
#include "ElaScrollPageArea.h"
#include "ElaSlider.h"
#include "ElaSpinBox.h"
#include "ElaText.h"
#include "ElaToggleButton.h"
#include "ElaToggleSwitch.h"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>

T_BaseComponents::T_BaseComponents(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaBaseComponents");

    // 顶部元素
    createCustomWidget("一些常用的基础组件被放置于此，可在此界面体验其效果并按需添加进项目中");

    _toggleSwitch = new ElaToggleSwitch(this);
    ElaScrollPageArea* toggleSwitchArea = new ElaScrollPageArea(this);
    QHBoxLayout* toggleSwitchLayout = new QHBoxLayout(toggleSwitchArea);
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
    QHBoxLayout* toggleButtonLayout = new QHBoxLayout(toggleButtonArea);
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
    QHBoxLayout* comboBoxLayout = new QHBoxLayout(comboBoxArea);
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
    QHBoxLayout* multiSelectComboBoxLayout = new QHBoxLayout(multiSelectComboBoxArea);
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
    _messageButton->setBarText("点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");

    _infoMessageButton = new ElaMessageButton("Info", this);
    _infoMessageButton->setBarTitle("Information");
    _infoMessageButton->setBarText("点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");
    _infoMessageButton->setMessageMode(ElaMessageBarType::Information);
    _infoMessageButton->setPositionPolicy(ElaMessageBarType::TopLeft);

    _warningMessageButton = new ElaMessageButton("Warning", this);
    _warningMessageButton->setBarTitle("Warning");
    _warningMessageButton->setBarText("点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");
    _warningMessageButton->setMessageMode(ElaMessageBarType::Warning);
    _warningMessageButton->setPositionPolicy(ElaMessageBarType::BottomLeft);

    _errorMessageButton = new ElaMessageButton("Error", this);
    _errorMessageButton->setBarTitle("Error");
    _errorMessageButton->setBarText("点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");
    _errorMessageButton->setMessageMode(ElaMessageBarType::Error);
    _errorMessageButton->setPositionPolicy(ElaMessageBarType::BottomRight);

    ElaScrollPageArea* messageButtonArea = new ElaScrollPageArea(this);
    QHBoxLayout* messageButtonLayout = new QHBoxLayout(messageButtonArea);
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
    QHBoxLayout* checkBoxLayout = new QHBoxLayout(checkBoxArea);
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
    QHBoxLayout* spinBoxLayout = new QHBoxLayout(spinBoxArea);
    ElaText* spinBoxText = new ElaText("ElaSpinBox", this);
    spinBoxText->setTextPixelSize(15);
    spinBoxLayout->addWidget(spinBoxText);
    spinBoxLayout->addWidget(_spinBox);
    spinBoxLayout->addStretch();

    ElaRadioButton* inlineButton = new ElaRadioButton("Inline", this);
    ElaRadioButton* compactButton = new ElaRadioButton("Compact", this);
    ElaRadioButton* sideButton = new ElaRadioButton("Side", this);
    ElaRadioButton* pmSideButton = new ElaRadioButton("PMSide", this);
    inlineButton->setChecked(true);
    QHBoxLayout* buttonModeLayout = new QHBoxLayout();
    ElaText* buttonModeText = new ElaText("按钮模式切换", this);
    buttonModeText->setWordWrap(false);
    buttonModeText->setTextPixelSize(15);
    buttonModeLayout->addWidget(buttonModeText);
    buttonModeLayout->addWidget(inlineButton);
    buttonModeLayout->addWidget(compactButton);
    buttonModeLayout->addWidget(sideButton);
    buttonModeLayout->addWidget(pmSideButton);

    spinBoxLayout->addLayout(buttonModeLayout);

    QButtonGroup* spinButtonGroup = new QButtonGroup(this);
    spinButtonGroup->addButton(inlineButton, 0);
    spinButtonGroup->addButton(compactButton, 1);
    spinButtonGroup->addButton(sideButton, 2);
    spinButtonGroup->addButton(pmSideButton, 3);
    connect(spinButtonGroup, QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled), this, [=](QAbstractButton* button, bool isToggled) {
        if (isToggled)
        {
            _spinBox->setButtonMode((ElaSpinBoxType::ButtonMode)spinButtonGroup->id(button));
        }
    });

    _slider = new ElaSlider(this);
    ElaScrollPageArea* sliderArea = new ElaScrollPageArea(this);
    QHBoxLayout* sliderLayout = new QHBoxLayout(sliderArea);
    ElaText* sliderText = new ElaText("ElaSlider", this);
    sliderText->setTextPixelSize(15);
    sliderLayout->addWidget(sliderText);
    sliderLayout->addWidget(_slider);
    sliderLayout->addStretch();

    _radioButton = new ElaRadioButton("RadioButton", this);
    ElaScrollPageArea* radioButtonArea = new ElaScrollPageArea(this);
    QHBoxLayout* radioButtonLayout = new QHBoxLayout(radioButtonArea);
    ElaText* radioButtonText = new ElaText("ElaRadioButton", this);
    radioButtonText->setTextPixelSize(15);
    radioButtonLayout->addWidget(radioButtonText);
    radioButtonLayout->addWidget(_radioButton);
    radioButtonLayout->addStretch();

    _progressBar = new ElaProgressBar(this);
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(0);
    ElaScrollPageArea* progressBarArea = new ElaScrollPageArea(this);
    QHBoxLayout* progressBarLayout = new QHBoxLayout(progressBarArea);
    ElaText* progressBarText = new ElaText("ElaProgressBar", this);
    progressBarText->setTextPixelSize(15);
    progressBarLayout->addWidget(progressBarText);
    progressBarLayout->addWidget(_progressBar);
    progressBarLayout->addStretch();

    _progressRing = new ElaProgressRing(this);
    _progressRing->setValue(30);
    _progressPercentRing = new ElaProgressRing(this);
    _progressPercentRing->setValue(50);
    _progressPercentRing->setValueDisplayMode(ElaProgressRingType::ValueDisplayMode::Percent);
    _progressBusyRing = new ElaProgressRing(this);
    _progressBusyRing->setIsBusying(true);
    _progressBusyTransparentRing = new ElaProgressRing(this);
    _progressBusyTransparentRing->setIsBusying(true);
    _progressBusyTransparentRing->setIsTransparent(true);
    ElaScrollPageArea* progressRingArea = new ElaScrollPageArea(this);
    progressRingArea->setFixedHeight(90);
    QHBoxLayout* progressRingLayout = new QHBoxLayout(progressRingArea);
    ElaText* progressRingText = new ElaText("ElaProgressRing", this);
    progressRingText->setTextPixelSize(15);
    progressRingLayout->addWidget(progressRingText);
    progressRingLayout->addWidget(_progressRing);
    progressRingLayout->addSpacing(10);
    progressRingLayout->addWidget(_progressPercentRing);
    progressRingLayout->addSpacing(10);
    progressRingLayout->addWidget(_progressBusyRing);
    progressRingLayout->addSpacing(10);
    progressRingLayout->addWidget(_progressBusyTransparentRing);
    progressRingLayout->addStretch();

    ElaPlainTextEdit* edit = new ElaPlainTextEdit(this);
    edit->setPlainText("这是一个ElaPlainTextEdit  暂时放在这里");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaBaseComponents");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->addWidget(toggleSwitchArea);
    centerLayout->addWidget(toggleButtonArea);
    centerLayout->addWidget(comboBoxArea);
    centerLayout->addWidget(multiSelectComboBoxArea);
    centerLayout->addWidget(messageButtonArea);
    centerLayout->addWidget(checkBoxArea);
    centerLayout->addWidget(spinBoxArea);
    centerLayout->addWidget(sliderArea);
    centerLayout->addWidget(radioButtonArea);
    centerLayout->addWidget(progressBarArea);
    centerLayout->addWidget(progressRingArea);
    centerLayout->addWidget(edit);
    centerLayout->addStretch();
    centerLayout->setContentsMargins(0, 0, 0, 0);
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
