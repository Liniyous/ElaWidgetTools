#include "T_Popup.h"

#include "ElaCalendar.h"
#include "ElaCalendarPicker.h"
#include "ElaCheckBox.h"
#include "ElaColorDialog.h"
#include "ElaContentDialog.h"
#include "ElaDrawerArea.h"
#include "ElaFlowLayout.h"
#include "ElaKeyBinder.h"
#include "ElaLineEdit.h"
#include "ElaMenu.h"
#include "ElaPushButton.h"
#include "ElaRoller.h"
#include "ElaRollerPicker.h"
#include "ElaTeachingTip.h"
#include "ElaText.h"
#include "ElaToolButton.h"

#include <QDateTime>
#include <QHBoxLayout>
#include <QVBoxLayout>

T_Popup::T_Popup(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("弹出与选择");
    createCustomWidget("展开菜单、选择日期与颜色，体验对话框和可展开区域。", "弹出与选择");
    auto* mainLayout = createPageLayout();
    auto* menuSection = addExampleSection(mainLayout, "菜单与对话框", "点击菜单项可查看选择结果，对话框用于需要确认的操作。");
    auto* popupLayout = new ElaFlowLayout(0, 16, 16);
    auto* menuResultText = new ElaText("菜单选择：尚未选择", this);
    _toolButton = new ElaToolButton(this);
    _toolButton->setIsTransparent(false);
    _toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _toolButton->setText("文件操作");
    _toolButton->setElaIcon(ElaIconType::FolderOpen);
    auto* menu = new ElaMenu(this);
    for (const QString& text: {QString("新建项目"), QString("保存项目"), QString("导出副本")})
    {
        connect(menu->addAction(text), &QAction::triggered, this, [menuResultText, text]() {
            menuResultText->setText("菜单选择：" + text);
        });
    }
    _toolButton->setMenu(menu);
    auto* confirmButton = new ElaPushButton("显示确认对话框", this);
    auto* confirmDialog = new ElaContentDialog(this);
    auto* confirmWidget = new QWidget(confirmDialog);
    auto* confirmLayout = new QVBoxLayout(confirmWidget);
    confirmLayout->setContentsMargins(24, 24, 24, 16);
    confirmLayout->setSpacing(8);
    auto* confirmTitleText = new ElaText("是否保存当前更改？", confirmWidget);
    confirmTitleText->setTextPixelSize(confirmTitleText->getTextPixelSize() + 6);
    confirmTitleText->setIsBold(true);
    auto* confirmDescriptionText = new ElaText("选择一个操作，结果会显示在示例区域。", confirmWidget);
    confirmLayout->addWidget(confirmTitleText);
    confirmLayout->addWidget(confirmDescriptionText);
    confirmDialog->setCentralWidget(confirmWidget);
    confirmDialog->setLeftButtonText("取消");
    confirmDialog->setMiddleButtonText("不保存");
    confirmDialog->setRightButtonText("保存");
    connect(confirmButton, &ElaPushButton::clicked, confirmDialog, &ElaContentDialog::exec);
    connect(confirmDialog, &ElaContentDialog::rightButtonClicked, this, [menuResultText]() {
        menuResultText->setText("对话框结果：保存更改");
    });
    connect(confirmDialog, &ElaContentDialog::middleButtonClicked, this, [menuResultText]() {
        menuResultText->setText("对话框结果：不保存");
    });
    popupLayout->addWidget(_toolButton);
    popupLayout->addWidget(confirmButton);
    menuSection->addLayout(popupLayout);
    menuSection->addWidget(menuResultText);

    auto* teachingSection = addExampleSection(mainLayout, "教学提示", "ElaTeachingTip 指向目标控件，支持自定义内容与点击外部关闭。");
    auto* teachingLayout = new ElaFlowLayout(0, 16, 16);
    auto* teachingButton = new ElaPushButton("功能引导", this);
    auto* customTipButton = new ElaPushButton("自定义内容", this);
    auto* transientTipButton = new ElaPushButton("轻量提示", this);
    auto* teachingResultText = new ElaText("提示中的按钮和自定义控件可直接交互。", this);

    auto* teachingTip = new ElaTeachingTip(this);
    teachingTip->setTargetWidget(teachingButton);
    teachingTip->setTitle("试试新的工作方式");
    teachingTip->setSubTitle("提示会指向相关控件，\n帮助你了解功能和操作方式。");
    teachingTip->setElaIcon(ElaIconType::Lightbulb);
    teachingTip->setActionButtonText("了解更多");
    teachingTip->setCloseButtonText("知道了");
    connect(teachingTip, &ElaTeachingTip::actionButtonClicked, this, [teachingTip, teachingResultText]() {
        teachingResultText->setText("已选择：了解更多");
        teachingTip->setIsOpen(false);
    });

    auto* customTip = new ElaTeachingTip(this);
    customTip->setTargetWidget(customTipButton);
    customTip->setTitle("调整提示内容");
    customTip->setSubTitle("可以在提示中直接使用输入框和选项。");
    customTip->setTipWidth(336);
    auto* customTipWidget = new QWidget;
    auto* customTipLayout = new QVBoxLayout(customTipWidget);
    customTipLayout->setContentsMargins(0, 0, 0, 0);
    customTipLayout->setSpacing(8);
    auto* displayNameEdit = new ElaLineEdit(customTipWidget);
    displayNameEdit->setPlaceholderText("输入显示名称");
    displayNameEdit->setText("我的工作区");
    auto* rememberCheckBox = new ElaCheckBox("下次继续显示提示", customTipWidget);
    rememberCheckBox->setChecked(true);
    customTipLayout->addWidget(displayNameEdit);
    customTipLayout->addWidget(rememberCheckBox);
    customTip->setCustomWidget(customTipWidget);
    customTip->setActionButtonText("应用");
    customTip->setCloseButtonText("取消");
    connect(customTip, &ElaTeachingTip::actionButtonClicked, this, [customTip, teachingResultText, displayNameEdit]() {
        teachingResultText->setText(QString("已应用：%1").arg(displayNameEdit->text()));
        customTip->setIsOpen(false);
    });

    auto* transientTip = new ElaTeachingTip(this);
    transientTip->setTargetWidget(transientTipButton);
    transientTip->setTitle("点击其他位置即可关闭");
    transientTip->setSubTitle("点击或滚动提示外部区域，浮层会自动收起。");
    transientTip->setIsLightDismissEnabled(true);
    const auto showTip = [teachingTip, customTip, transientTip](ElaTeachingTip* current) {
        const bool isOpening = !current->getIsOpen();
        for (auto* tip: {teachingTip, customTip, transientTip})
        {
            tip->setIsOpen(tip == current && isOpening);
        }
    };
    connect(teachingButton, &ElaPushButton::clicked, teachingTip, [showTip, teachingTip]() {
        showTip(teachingTip);
    });
    connect(customTipButton, &ElaPushButton::clicked, customTip, [showTip, customTip]() {
        showTip(customTip);
    });
    connect(transientTipButton, &ElaPushButton::clicked, transientTip, [showTip, transientTip]() {
        showTip(transientTip);
    });
    teachingLayout->addWidget(teachingButton);
    teachingLayout->addWidget(customTipButton);
    teachingLayout->addWidget(transientTipButton);
    teachingSection->addLayout(teachingLayout);
    teachingSection->addWidget(teachingResultText);

    auto* colorSection = addExampleSection(mainLayout, "颜色选择", "ElaColorDialog 支持颜色预览、RGB / HSV 输入和自定义色位。");
    _colorDialog = new ElaColorDialog(this);
    auto* colorDialogButton = new ElaPushButton("选择颜色", this);
    auto* colorPreviewButton = new ElaPushButton(this);
    colorPreviewButton->setFixedSize(40, 32);
    colorPreviewButton->setAccessibleName("当前颜色预览");
    auto* colorText = new ElaText(this);
    auto updateColor = [colorPreviewButton, colorText](const QColor& color) {
        colorPreviewButton->setLightDefaultColor(color);
        colorPreviewButton->setLightHoverColor(color);
        colorPreviewButton->setLightPressColor(color);
        colorPreviewButton->setDarkDefaultColor(color);
        colorPreviewButton->setDarkHoverColor(color);
        colorPreviewButton->setDarkPressColor(color);
        colorPreviewButton->update();
        colorText->setText(color.name().toUpper());
    };
    updateColor(_colorDialog->getCurrentColor());
    connect(_colorDialog, &ElaColorDialog::colorSelected, this, updateColor);
    connect(colorDialogButton, &ElaPushButton::clicked, _colorDialog, &ElaColorDialog::exec);
    connect(colorPreviewButton, &ElaPushButton::clicked, _colorDialog, &ElaColorDialog::exec);
    auto* colorLayout = new QHBoxLayout();
    colorLayout->setSpacing(16);
    colorLayout->addWidget(colorDialogButton);
    colorLayout->addWidget(colorPreviewButton);
    colorLayout->addWidget(colorText);
    colorLayout->addStretch();
    colorSection->addLayout(colorLayout);

    auto* calendarSection = addExampleSection(mainLayout, "日期选择", "ElaCalendarPicker 与 ElaCalendar 联动，支持月份切换和直接选择日期。");
    _calendar = new ElaCalendar(this);
    _calendarPicker = new ElaCalendarPicker(this);
    auto* dateText = new ElaText(QDate::currentDate().toString("yyyy-MM-dd"), this);
    auto* dateOptionsWidget = createLabeledControl("弹出式日期选择", _calendarPicker);
    dateOptionsWidget->layout()->addWidget(dateText);
    auto* calendarLayout = new ElaFlowLayout(0, 24, 16);
    calendarLayout->addWidget(_calendar);
    calendarLayout->addWidget(dateOptionsWidget);
    connect(_calendar, &ElaCalendar::clicked, this, [this, dateText](const QDate& date) {
        _calendarPicker->setSelectedDate(date);
        dateText->setText(date.toString("yyyy-MM-dd"));
    });
    connect(_calendarPicker, &ElaCalendarPicker::selectedDateChanged, this, [this, dateText](const QDate& date) {
        _calendar->setSelectedDate(date);
        dateText->setText(date.toString("yyyy-MM-dd"));
    });
    calendarSection->addLayout(calendarLayout);

    auto* rollerSection = addExampleSection(mainLayout, "滚轮选择", "ElaRoller · ElaRollerPicker，使用滚轮或打开选择器设置值。");
    _roller = new ElaRoller(this);
    QStringList months;
    QStringList hours;
    QStringList minutes;
    for (int index = 1; index <= 12; ++index)
    {
        months.append(QString("%1 月").arg(index));
    }
    for (int index = 0; index < 24; ++index)
    {
        hours.append(QString("%1").arg(index, 2, 10, QChar('0')));
    }
    for (int index = 0; index < 60; ++index)
    {
        minutes.append(QString("%1").arg(index, 2, 10, QChar('0')));
    }
    _roller->setItemList(months);
    _timeRollerPicker = new ElaRollerPicker(this);
    _timeRollerPicker->addRoller(hours);
    _timeRollerPicker->addRoller(minutes);
    _timeRollerPicker->setCurrentData({QTime::currentTime().toString("HH"), QTime::currentTime().toString("mm")});
    _clockRollerPicker = new ElaRollerPicker(this);
    _clockRollerPicker->addRoller({"工作日", "周末", "每天"}, false);
    _clockRollerPicker->setRollerWidth(0, 180);
    auto* rollerLayout = new ElaFlowLayout(0, 28, 16);
    rollerLayout->addWidget(createLabeledControl("月份", _roller));
    rollerLayout->addWidget(createLabeledControl("24 小时时间", _timeRollerPicker));
    rollerLayout->addWidget(createLabeledControl("提醒周期", _clockRollerPicker));
    rollerSection->addLayout(rollerLayout);

    auto* keySection = addExampleSection(mainLayout, "快捷键绑定", "点击 ElaKeyBinder 后按下组合键，在对话框中确认或重置。");
    _keyBinder = new ElaKeyBinder(this);
    _keyBinder->setFixedWidth(260);
    keySection->addWidget(_keyBinder, 0, Qt::AlignLeft);

    auto* drawerSection = addExampleSection(mainLayout, "可展开区域", "ElaDrawerArea 将次要选项折叠收纳，点击标题展开。");
    _drawer = new ElaDrawerArea(this);
    auto* headerWidget = new QWidget(this);
    auto* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(16, 8, 16, 8);
    headerLayout->addWidget(new ElaText("更多偏好设置", this));
    headerLayout->addStretch();
    _drawer->setDrawerHeader(headerWidget);
    for (const QString& title: {QString("启动时恢复上次工作区"), QString("完成操作后显示消息"), QString("允许后台自动更新")})
    {
        auto* optionWidget = new QWidget(this);
        optionWidget->setFixedHeight(54);
        auto* optionLayout = new QHBoxLayout(optionWidget);
        optionLayout->setContentsMargins(20, 8, 20, 8);
        optionLayout->addWidget(new ElaCheckBox(title, this));
        optionLayout->addStretch();
        _drawer->addDrawer(optionWidget);
    }
    drawerSection->addWidget(_drawer);
}

T_Popup::~T_Popup()
{
}
