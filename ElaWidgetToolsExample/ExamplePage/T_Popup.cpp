#include "T_Popup.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaCalendar.h"
#include "ElaCalendarPicker.h"
#include "ElaColorDialog.h"
#include "ElaKeyBinder.h"
#include "ElaMenu.h"
#include "ElaPushButton.h"
#include "ElaRoller.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaToolButton.h"
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
    rollerLayout->addStretch();

    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(toolButtonArea);
    centerVLayout->addWidget(colorDialogArea);
    centerVLayout->addWidget(calendarPickerArea);
    centerVLayout->addWidget(_calendar);
    centerVLayout->addWidget(keyBinderArea);
    centerVLayout->addWidget(rollerArea);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_Popup::~T_Popup()
{
}
