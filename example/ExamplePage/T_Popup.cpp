#include "T_Popup.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaMenu.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaToolButton.h"
T_Popup::T_Popup(QWidget* parent)
    : ElaScrollPage(parent)
{
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaPopup");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);

    _toolButton = new ElaToolButton(this);
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
    centerVLayout->addWidget(toolButtonArea);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, true, 0);
}

T_Popup::~T_Popup()
{
}
