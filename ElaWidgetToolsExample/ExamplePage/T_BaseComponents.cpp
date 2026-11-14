#include "T_BaseComponents.h"

#include "ElaCheckBox.h"
#include "ElaComboBox.h"
#include "ElaDoubleSpinBox.h"
#include "ElaFlowLayout.h"
#include "ElaLineEdit.h"
#include "ElaMessageButton.h"
#include "ElaMultiSelectComboBox.h"
#include "ElaPlainTextEdit.h"
#include "ElaProgressBar.h"
#include "ElaProgressRing.h"
#include "ElaPushButton.h"
#include "ElaRadioButton.h"
#include "ElaSlider.h"
#include "ElaSpinBox.h"
#include "ElaText.h"
#include "ElaToggleButton.h"
#include "ElaToggleSwitch.h"
#include "ElaToolButton.h"

#include <QButtonGroup>
#include <QVBoxLayout>

T_BaseComponents::T_BaseComponents(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("基础控件");
    createCustomWidget("直接试用按钮、选择、输入与进度控件，观察状态变化和操作结果。", "基础控件");
    auto* mainLayout = createPageLayout();

    auto* buttonsSection = addExampleSection(mainLayout, "按钮与操作", "ElaPushButton · ElaToolButton · ElaToggleButton");
    auto* buttonLayout = new ElaFlowLayout(0, 16, 16);
    auto* normalButton = new ElaPushButton("普通按钮", this);
    auto* accentButton = new ElaPushButton("保存更改", this);
    accentButton->setIsAccent(true);
    auto* disabledButton = new ElaPushButton("不可用", this);
    disabledButton->setEnabled(false);
    auto* iconButton = new ElaToolButton(this);
    iconButton->setElaIcon(ElaIconType::FloppyDisk);
    iconButton->setText("保存");
    iconButton->setIsTransparent(false);
    iconButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _toggleButton = new ElaToggleButton("固定到顶部", this);
    _toggleButton->setFixedSize(qMax(100, _toggleButton->fontMetrics().horizontalAdvance(_toggleButton->getText()) + 24), normalButton->height());
    iconButton->setFixedHeight(normalButton->height());
    for (auto* button: {normalButton, accentButton, disabledButton})
    {
        button->setMinimumWidth(100);
        buttonLayout->addWidget(button);
    }
    buttonLayout->addWidget(iconButton);
    buttonLayout->addWidget(_toggleButton);
    auto* buttonResultText = new ElaText("操作结果：等待点击", this);
    for (auto* button: {normalButton, accentButton})
    {
        connect(button, &ElaPushButton::clicked, this, [button, buttonResultText]() {
            buttonResultText->setText(QString("操作结果：已点击 %1").arg(button->text()));
        });
    }
    connect(iconButton, &ElaToolButton::clicked, this, [buttonResultText]() {
        buttonResultText->setText("操作结果：已保存");
    });
    connect(_toggleButton, &ElaToggleButton::toggled, this, [buttonResultText](bool checked) {
        buttonResultText->setText(checked ? "操作结果：已固定" : "操作结果：已取消固定");
    });
    buttonsSection->addLayout(buttonLayout);
    buttonsSection->addWidget(buttonResultText);

    auto* selectionSection = addExampleSection(mainLayout, "开关与选择", "开关控制即时设置，复选框支持多个选项，单选按钮用于互斥选择。");
    auto* selectionLayout = new ElaFlowLayout(0, 28, 18);
    _toggleSwitch = new ElaToggleSwitch(this);
    _checkBox = new ElaCheckBox("接收通知", this);
    auto* partialCheckBox = new ElaCheckBox("部分选中", this);
    partialCheckBox->setTristate(true);
    partialCheckBox->setCheckState(Qt::PartiallyChecked);
    selectionLayout->addWidget(createLabeledControl("ElaToggleSwitch", _toggleSwitch));
    selectionLayout->addWidget(createLabeledControl("ElaCheckBox", _checkBox));
    selectionLayout->addWidget(createLabeledControl("三态复选框", partialCheckBox));
    auto* radioLayout = new ElaFlowLayout(0, 20, 12);
    auto* radioGroup = new QButtonGroup(this);
    _radioButton = new ElaRadioButton("每天", this);
    for (auto* radioButton: {_radioButton, new ElaRadioButton("工作日", this), new ElaRadioButton("周末", this)})
    {
        radioGroup->addButton(radioButton);
        radioLayout->addWidget(radioButton);
    }
    _radioButton->setChecked(true);
    selectionSection->addLayout(selectionLayout);
    selectionSection->addWidget(new ElaText("提醒频率 · ElaRadioButton", this));
    selectionSection->addLayout(radioLayout);

    auto* comboSection = addExampleSection(mainLayout, "下拉选择", "单选确定一个选项，多选以标签展示结果，可直接移除标签。");
    auto* comboLayout = new ElaFlowLayout(0, 24, 16);
    _comboBox = new ElaComboBox(this);
    _comboBox->setFixedWidth(240);
    _comboBox->addItems({"设计模式", "开发模式", "阅读模式"});
    _multiSelectComboBox = new ElaMultiSelectComboBox(this);
    _multiSelectComboBox->setFixedWidth(240);
    _multiSelectComboBox->addItems({"界面设计", "交互开发", "质量验证", "跨平台兼容", "文档", "性能优化"});
    _multiSelectComboBox->setCurrentSelection(QStringList{"界面设计", "文档"});
    comboLayout->addWidget(createLabeledControl("ElaComboBox", _comboBox));
    comboLayout->addWidget(createLabeledControl("ElaMultiSelectComboBox", _multiSelectComboBox));
    auto* comboResultText = new ElaText("当前模式：设计模式", this);
    auto* multiResultText = new ElaText("已选标签：界面设计、文档", this);
    multiResultText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    connect(_comboBox, &ElaComboBox::currentTextChanged, this, [comboResultText](const QString& text) {
        comboResultText->setText("当前模式：" + text);
    });
    connect(_multiSelectComboBox, &ElaMultiSelectComboBox::currentTextListChanged, this, [multiResultText](const QStringList& values) {
        multiResultText->setText(values.isEmpty() ? "已选标签：无" : "已选标签：" + values.join("、"));
    });
    auto* disableComboCheckBox = new ElaCheckBox("禁用本组选择控件", this);
    connect(disableComboCheckBox, &ElaCheckBox::toggled, this, [this](bool checked) {
        _comboBox->setDisabled(checked);
        _multiSelectComboBox->setDisabled(checked);
    });
    comboSection->addLayout(comboLayout);
    comboSection->addWidget(comboResultText);
    comboSection->addWidget(multiResultText);
    comboSection->addWidget(disableComboCheckBox);

    auto* textSection = addExampleSection(mainLayout, "文本输入", "ElaLineEdit · ElaPlainTextEdit，点击输入框可观察焦点底线的展开。");
    auto* editLayout = new ElaFlowLayout(0, 24, 16);
    auto* projectEdit = new ElaLineEdit(this);
    projectEdit->setFixedWidth(240);
    projectEdit->setPlaceholderText("输入项目名称");
    projectEdit->setClearButtonEnabled(true);
    auto* passwordEdit = new ElaLineEdit(this);
    passwordEdit->setFixedWidth(240);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("输入密码");
    editLayout->addWidget(createLabeledControl("项目名称", projectEdit));
    editLayout->addWidget(createLabeledControl("密码输入", passwordEdit));
    auto* descriptionEdit = new ElaPlainTextEdit(this);
    descriptionEdit->setPlaceholderText("填写项目说明，支持多行输入和右键编辑菜单。");
    descriptionEdit->setFixedHeight(120);
    descriptionEdit->setPlainText("这是一个多行文本输入示例。\n可以自由编辑内容，或使用右键菜单进行常用编辑操作。");
    textSection->addLayout(editLayout);
    textSection->addWidget(new ElaText("项目说明 · ElaPlainTextEdit", this));
    textSection->addWidget(descriptionEdit);

    auto* numberSection = addExampleSection(mainLayout, "数值输入", "ElaSpinBox · ElaDoubleSpinBox，可切换整数输入框的按钮布局。");
    auto* numberLayout = new ElaFlowLayout(0, 24, 16);
    _spinBox = new ElaSpinBox(this);
    _spinBox->setRange(0, 100);
    _spinBox->setValue(24);
    _spinBox->setFixedWidth(180);
    auto* decimalSpinBox = new ElaDoubleSpinBox(this);
    decimalSpinBox->setRange(0, 100);
    decimalSpinBox->setDecimals(1);
    decimalSpinBox->setSingleStep(0.5);
    decimalSpinBox->setValue(3.5);
    decimalSpinBox->setFixedWidth(180);
    auto* modeComboBox = new ElaComboBox(this);
    modeComboBox->setFixedWidth(180);
    modeComboBox->addItem("内嵌按钮", int(ElaSpinBoxType::Inline));
    modeComboBox->addItem("紧凑按钮", int(ElaSpinBoxType::Compact));
    modeComboBox->addItem("两侧箭头", int(ElaSpinBoxType::Side));
    modeComboBox->addItem("两侧加减", int(ElaSpinBoxType::PMSide));
    connect(modeComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [this, modeComboBox](int index) {
        _spinBox->setButtonMode(static_cast<ElaSpinBoxType::ButtonMode>(modeComboBox->itemData(index).toInt()));
    });
    numberLayout->addWidget(createLabeledControl("整数", _spinBox));
    numberLayout->addWidget(createLabeledControl("小数", decimalSpinBox));
    numberLayout->addWidget(createLabeledControl("按钮布局", modeComboBox));
    numberSection->addLayout(numberLayout);

    auto* progressSection = addExampleSection(mainLayout, "滑块与进度", "拖动滑块，同时观察进度条和进度环；不确定进度用于等待中的操作。");
    _slider = new ElaSlider(this);
    _slider->setRange(0, 100);
    _slider->setValue(45);
    _slider->setAccessibleName("演示进度");
    _progressBar = new ElaProgressBar(this);
    _progressBar->setRange(0, 100);
    _progressBar->setValue(45);
    _progressRing = new ElaProgressRing(this);
    _progressRing->setValue(45);
    _progressPercentRing = new ElaProgressRing(this);
    _progressPercentRing->setValue(45);
    _progressPercentRing->setValueDisplayMode(ElaProgressRingType::Percent);
    _progressBusyRing = new ElaProgressRing(this);
    _progressBusyRing->setIsBusying(true);
    _progressBusyTransparentRing = new ElaProgressRing(this);
    _progressBusyTransparentRing->setIsBusying(true);
    _progressBusyTransparentRing->setIsTransparent(true);
    auto* progressLayout = new ElaFlowLayout(0, 28, 16);
    for (auto* ring: {_progressRing, _progressPercentRing, _progressBusyRing, _progressBusyTransparentRing})
    {
        ring->setFixedSize(54, 54);
    }
    progressLayout->addWidget(createLabeledControl("数值", _progressRing));
    progressLayout->addWidget(createLabeledControl("百分比", _progressPercentRing));
    progressLayout->addWidget(createLabeledControl("等待中", _progressBusyRing));
    progressLayout->addWidget(createLabeledControl("透明背景", _progressBusyTransparentRing));
    connect(_slider, &ElaSlider::valueChanged, this, [this](int value) {
        _progressBar->setValue(value);
        _progressRing->setValue(value);
        _progressPercentRing->setValue(value);
    });
    progressSection->addWidget(_slider);
    progressSection->addWidget(_progressBar);
    progressSection->addLayout(progressLayout);

    auto* messageSection = addExampleSection(mainLayout, "状态反馈", "ElaMessageButton，点击不同按钮查看成功、提示、警告和错误消息。");
    auto* messageLayout = new ElaFlowLayout(0, 12, 12);
    _messageButton = new ElaMessageButton("成功", this);
    _infoMessageButton = new ElaMessageButton("提示", this);
    _warningMessageButton = new ElaMessageButton("警告", this);
    _errorMessageButton = new ElaMessageButton("错误", this);
    _infoMessageButton->setMessageMode(ElaMessageBarType::Information);
    _warningMessageButton->setMessageMode(ElaMessageBarType::Warning);
    _errorMessageButton->setMessageMode(ElaMessageBarType::Error);
    for (auto* button: {_messageButton, _infoMessageButton, _warningMessageButton, _errorMessageButton})
    {
        button->setBarTitle(button->text());
        button->setBarText("这是一条状态消息示例，可用于反馈操作结果。");
        button->setMessageTargetWidget(this);
        button->setDisplayMsec(2500);
        messageLayout->addWidget(button);
    }
    messageSection->addLayout(messageLayout);
}

T_BaseComponents::~T_BaseComponents()
{
}
