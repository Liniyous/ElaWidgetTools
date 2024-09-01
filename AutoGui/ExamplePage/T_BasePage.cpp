#include "T_BasePage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaMenu.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"
T_BasePage::T_BasePage(QWidget* parent)
    : ElaScrollPage(parent)
{
}

T_BasePage::~T_BasePage()
{
}

QVBoxLayout* T_BasePage::createTopLayout(QString desText)
{
    // 顶部元素
    ElaText* subTitleText = new ElaText(this);
    subTitleText->setText("https://github.com/Liniyous/ElaWidgetTools");
    subTitleText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    subTitleText->setTextPixelSize(11);

    ElaToolButton* documentationButton = new ElaToolButton(this);
    documentationButton->setFixedHeight(35);
    documentationButton->setIsTransparent(false);
    documentationButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    documentationButton->setText("Documentation");
    documentationButton->setElaIcon(ElaIconType::FileDoc);
    ElaMenu* documentationMenu = new ElaMenu(this);
    documentationMenu->addElaIconAction(ElaIconType::CardsBlank, "CardsBlank");
    documentationMenu->addElaIconAction(ElaIconType::EarthAmericas, "EarthAmericas");
    documentationButton->setMenu(documentationMenu);

    ElaToolButton* sourceButton = new ElaToolButton(this);
    sourceButton->setFixedHeight(35);
    sourceButton->setIsTransparent(false);
    sourceButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    sourceButton->setText("Source");
    sourceButton->setElaIcon(ElaIconType::NfcSymbol);
    ElaMenu* sourceMenu = new ElaMenu(this);
    sourceMenu->addElaIconAction(ElaIconType::FireBurner, "FireBurner");
    sourceMenu->addElaIconAction(ElaIconType::Galaxy, "Galaxy~~~~");
    sourceButton->setMenu(sourceMenu);

    ElaToolButton* themeButton = new ElaToolButton(this);
    themeButton->setFixedSize(35, 35);
    themeButton->setIsTransparent(false);
    themeButton->setElaIcon(ElaIconType::MoonStars);
    connect(themeButton, &ElaToolButton::clicked, this, [=]() {
        eTheme->setThemeMode(eTheme->getThemeMode() == ElaThemeType::Light ? ElaThemeType::Dark : ElaThemeType::Light);
    });

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(documentationButton);
    buttonLayout->addSpacing(5);
    buttonLayout->addWidget(sourceButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(themeButton);
    buttonLayout->addSpacing(15);

    ElaText* descText = new ElaText(this);
    descText->setText(desText);
    descText->setTextPixelSize(13);

    QVBoxLayout* topLayout = new QVBoxLayout();
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(subTitleText);
    topLayout->addSpacing(5);
    topLayout->addLayout(buttonLayout);
    topLayout->addSpacing(5);
    topLayout->addWidget(descText);
    return topLayout;
}
