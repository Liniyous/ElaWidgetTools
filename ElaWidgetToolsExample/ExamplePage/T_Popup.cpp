#include "T_Popup.h"

#include "ElaCalendar.h"
#include "ElaCalendarPicker.h"
#include "ElaCheckBox.h"
#include "ElaColorDialog.h"
#include "ElaDrawerArea.h"
#include "ElaKeyBinder.h"
#include "ElaMenu.h"
#include "ElaPushButton.h"
#include "ElaRoller.h"
#include "ElaRollerPicker.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaToggleSwitch.h"
#include "ElaToolButton.h"
#include <QDateTime>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

T_Popup::T_Popup(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaPopup");

    // 顶部元素
    createCustomWidget("一些常用的弹出组件被放置于此，可在此界面体验其效果并按需添加进项目中");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaPopup");

    _toolButton = new ElaToolButton(this);
    _toolButton->setIsTransparent(false);
    _toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    _toolButton->setText("ElaToolButton");

    ElaMenu* menu = new ElaMenu(this);
    menu->addElaIconAction(ElaIconType::JackOLantern, "JackOLantern");
    menu->addElaIconAction(ElaIconType::LacrosseStick, "LacrosseStick");
    _toolButton->setMenu(menu);
    _toolButton->setElaIcon(ElaIconType::Broom);

    ElaScrollPageArea* toolButtonArea = new ElaScrollPageArea(this);
    QHBoxLayout* toolButtonLayout = new QHBoxLayout(toolButtonArea);
    ElaText* toolButtonText = new ElaText("ElaToolButton", this);
    toolButtonText->setTextPixelSize(15);
    toolButtonLayout->addWidget(toolButtonText);
    toolButtonLayout->addWidget(_toolButton);
    toolButtonLayout->addStretch();

    _colorDialog = new ElaColorDialog(this);
    ElaText* colorText = new ElaText(_colorDialog->getCurrentColorRGB(), this);
    colorText->setTextPixelSize(15);
    ElaPushButton* colorDialogButton = new ElaPushButton(this);
    colorDialogButton->setFixedSize(35, 35);
    colorDialogButton->setLightDefaultColor(_colorDialog->getCurrentColor());
    colorDialogButton->setLightHoverColor(_colorDialog->getCurrentColor());
    colorDialogButton->setLightPressColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkDefaultColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkHoverColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkPressColor(_colorDialog->getCurrentColor());
    connect(colorDialogButton, &ElaPushButton::clicked, this, [=]() {
        _colorDialog->exec();
    });
    connect(_colorDialog, &ElaColorDialog::colorSelected, this, [=](const QColor& color) {
        colorDialogButton->setLightDefaultColor(color);
        colorDialogButton->setLightHoverColor(color);
        colorDialogButton->setLightPressColor(color);
        colorDialogButton->setDarkDefaultColor(color);
        colorDialogButton->setDarkHoverColor(color);
        colorDialogButton->setDarkPressColor(color);
        colorText->setText(_colorDialog->getCurrentColorRGB());
    });

    ElaScrollPageArea* colorDialogArea = new ElaScrollPageArea(this);
    QHBoxLayout* colorDialogLayout = new QHBoxLayout(colorDialogArea);
    ElaText* colorDialogText = new ElaText("ElaColorDialog", this);
    colorDialogText->setTextPixelSize(15);
    colorDialogLayout->addWidget(colorDialogText);
    colorDialogLayout->addWidget(colorDialogButton);
    colorDialogLayout->addWidget(colorText);
    colorDialogLayout->addStretch();

    _calendar = new ElaCalendar(this);

    _calendarPicker = new ElaCalendarPicker(this);
    ElaScrollPageArea* calendarPickerArea = new ElaScrollPageArea(this);
    QHBoxLayout* calendarPickerLayout = new QHBoxLayout(calendarPickerArea);
    ElaText* calendarPickerText = new ElaText("ElaCalendarPicker", this);
    calendarPickerText->setTextPixelSize(15);
    calendarPickerLayout->addWidget(calendarPickerText);
    calendarPickerLayout->addWidget(_calendarPicker);
    calendarPickerLayout->addStretch();

    _keyBinder = new ElaKeyBinder(this);
    ElaScrollPageArea* keyBinderArea = new ElaScrollPageArea(this);
    QHBoxLayout* keyBinderLayout = new QHBoxLayout(keyBinderArea);
    ElaText* keyBinderText = new ElaText("ElaKeyBinder", this);
    keyBinderText->setTextPixelSize(15);
    keyBinderLayout->addWidget(keyBinderText);
    keyBinderLayout->addWidget(_keyBinder);
    keyBinderLayout->addStretch();

    _roller = new ElaRoller(this);
    QStringList rollerItemList;
    for (int i = 0; i < 100; i++)
    {
        rollerItemList.append(QString::number(i + 1));
    }
    _roller->setItemList(rollerItemList);
    ElaScrollPageArea* rollerArea = new ElaScrollPageArea(this);
    rollerArea->setFixedHeight(220);
    QHBoxLayout* rollerLayout = new QHBoxLayout(rollerArea);
    ElaText* rollerText = new ElaText("ElaRoller", this);
    rollerText->setTextPixelSize(15);
    rollerLayout->addWidget(rollerText);
    rollerLayout->addWidget(_roller);
    rollerLayout->addSpacing(30);

    ElaText* rollerPickerText = new ElaText("ElaRollerPicker", this);
    rollerPickerText->setTextPixelSize(15);
    rollerLayout->addWidget(rollerPickerText);

    QTime currentTime = QTime::currentTime();
    QString currentHour = QString("%1").arg(currentTime.hour(), 2, 10, QChar('0'));
    QString currentMinute = QString("%1").arg(currentTime.minute(), 2, 10, QChar('0'));
    _timeRollerPicker = new ElaRollerPicker(this);
    QStringList hourItemList;
    for (int i = 0; i < 24; i++)
    {
        hourItemList.append(QString("%1").arg(i, 2, 10, QChar('0')));
    }
    QStringList minuteList;
    for (int i = 0; i < 61; i++)
    {
        minuteList.append(QString("%1").arg(i, 2, 10, QChar('0')));
    }
    _timeRollerPicker->addRoller(hourItemList);
    _timeRollerPicker->addRoller(minuteList);
    _timeRollerPicker->addRoller({"AM", "PM"}, false);
    _timeRollerPicker->setCurrentData({currentHour,
                                       currentMinute,
                                       currentTime.hour() >= 12 ? "PM" : "AM"});

    _clockRollerPicker = new ElaRollerPicker(this);
    _clockRollerPicker->addRoller(hourItemList);
    _clockRollerPicker->addRoller(minuteList);
    _clockRollerPicker->setRollerWidth(0, 135);
    _clockRollerPicker->setRollerWidth(1, 135);
    _clockRollerPicker->setCurrentData({currentHour,
                                        currentMinute});

    QVBoxLayout* rollerPickerLayout = new QVBoxLayout();
    rollerPickerLayout->addWidget(_timeRollerPicker);
    rollerPickerLayout->addWidget(_clockRollerPicker);
    rollerLayout->addLayout(rollerPickerLayout);
    rollerLayout->addStretch();

    _drawer = new ElaDrawerArea(this);
    QWidget* drawerHeader = new QWidget(this);
    QHBoxLayout* drawerHeaderLayout = new QHBoxLayout(drawerHeader);
    ElaText* drawerIcon = new ElaText(this);
    drawerIcon->setTextPixelSize(15);
    drawerIcon->setElaIcon(ElaIconType::MessageArrowDown);
    drawerIcon->setFixedSize(25, 25);
    ElaText* drawerText = new ElaText("ElaDrawer", this);
    drawerText->setTextPixelSize(15);

    ElaToggleSwitch* drawerSwitch = new ElaToggleSwitch(this);
    ElaText* drawerSwitchText = new ElaText("关", this);
    drawerSwitchText->setTextPixelSize(15);
    connect(drawerSwitch, &ElaToggleSwitch::toggled, this, [=](bool toggled) {
        if (toggled)
        {
            drawerSwitchText->setText("开");
            _drawer->expand();
        }
        else
        {
            drawerSwitchText->setText("关");
            _drawer->collapse();
        }
    });
    connect(_drawer, &ElaDrawerArea::expandStateChanged, this, [=](bool isExpand) {
        drawerSwitch->setIsToggled(isExpand);
    });

    drawerHeaderLayout->addWidget(drawerIcon);
    drawerHeaderLayout->addWidget(drawerText);
    drawerHeaderLayout->addStretch();
    drawerHeaderLayout->addWidget(drawerSwitchText);
    drawerHeaderLayout->addWidget(drawerSwitch);

    _drawer->setDrawerHeader(drawerHeader);
    QWidget* drawerWidget1 = new QWidget(this);
    drawerWidget1->setFixedHeight(75);
    QHBoxLayout* drawerWidget1Layout = new QHBoxLayout(drawerWidget1);
    ElaCheckBox* drawerCheckBox1 = new ElaCheckBox("测试窗口1", this);
    drawerWidget1Layout->addSpacing(60);
    drawerWidget1Layout->addWidget(drawerCheckBox1);

    QWidget* drawerWidget2 = new QWidget(this);
    drawerWidget2->setFixedHeight(75);
    QHBoxLayout* drawerWidget2Layout = new QHBoxLayout(drawerWidget2);
    ElaCheckBox* drawerCheckBox2 = new ElaCheckBox("测试窗口2", this);
    drawerWidget2Layout->addSpacing(60);
    drawerWidget2Layout->addWidget(drawerCheckBox2);

    QWidget* drawerWidget3 = new QWidget(this);
    drawerWidget3->setFixedHeight(75);
    QHBoxLayout* drawerWidget3Layout = new QHBoxLayout(drawerWidget3);
    ElaCheckBox* drawerCheckBox3 = new ElaCheckBox("测试窗口3", this);
    drawerWidget3Layout->addSpacing(60);
    drawerWidget3Layout->addWidget(drawerCheckBox3);

    _drawer->addDrawer(drawerWidget1);
    _drawer->addDrawer(drawerWidget2);
    _drawer->addDrawer(drawerWidget3);

    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(toolButtonArea);
    centerVLayout->addWidget(colorDialogArea);
    centerVLayout->addWidget(calendarPickerArea);
    centerVLayout->addWidget(_calendar);
    centerVLayout->addWidget(keyBinderArea);
    centerVLayout->addWidget(_drawer);
    centerVLayout->addWidget(rollerArea);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_Popup::~T_Popup()
{
}
