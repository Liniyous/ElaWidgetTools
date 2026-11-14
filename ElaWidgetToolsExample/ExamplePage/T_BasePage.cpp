#include "T_BasePage.h"

#include "ElaApplication.h"
#include "ElaImageCard.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"

#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QUrl>
#include <QVBoxLayout>
#include <QVariant>

T_BasePage::T_BasePage(QWidget* parent)
    : ElaScrollPage(parent)
{
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=]() {
        if (!parent)
        {
            update();
        }
    });
    setContentsMargins(20, 5, 0, 0);
}

T_BasePage::~T_BasePage()
{
}

void T_BasePage::createCustomWidget(const QString& description, const QString& title)
{
    // 展示页显式启用统一页头 Home 沿用自己的布局
    setTitleVisible(false);
    setContentsMargins(28, 12, 20, 0);
    auto* headerWidget = new QWidget(this);
    auto* titleText = new ElaText(title.isEmpty() ? windowTitle() : title, this);
    titleText->setTextPixelSize(eApp->getFontPixelSize() + 14);
    titleText->setIsBold(true);
    titleText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

    auto* themeButton = new ElaToolButton(this);
    themeButton->setFixedSize(32, 32);
    themeButton->setIsTransparent(false);
    themeButton->setElaIcon(ElaIconType::MoonStars);
    themeButton->setToolTip("切换浅色与深色主题");
    themeButton->setAccessibleName("切换主题");
    connect(themeButton, &ElaToolButton::clicked, this, []() {
        eTheme->setThemeMode(eTheme->getThemeMode() == ElaThemeType::Light ? ElaThemeType::Dark : ElaThemeType::Light);
    });

    const QString sourcePath = QFileInfo(__FILE__).absoluteDir().filePath(QString(metaObject()->className()) + ".cpp");
    auto* sourceButton = new ElaToolButton(this);
    sourceButton->setText("示例源码");
    sourceButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    sourceButton->setElaIcon(ElaIconType::Code);
    sourceButton->setIsTransparent(false);
    sourceButton->setVisible(QFileInfo::exists(sourcePath));
    connect(sourceButton, &ElaToolButton::clicked, this, [sourcePath]() {
        QDesktopServices::openUrl(QUrl::fromLocalFile(sourcePath));
    });

    auto* titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(12);
    titleLayout->addWidget(titleText, 1);
    titleLayout->addWidget(sourceButton);
    titleLayout->addWidget(themeButton);
    auto* descriptionText = new ElaText(description, this);
    descriptionText->setTextPixelSize(eApp->getFontPixelSize());
    descriptionText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    auto* mainLayout = new QVBoxLayout(headerWidget);
    mainLayout->setContentsMargins(0, 0, 0, 20);
    mainLayout->setSpacing(10);
    mainLayout->addLayout(titleLayout);
    mainLayout->addWidget(descriptionText);
    setTopCustomWidget(headerWidget);
}

QVBoxLayout* T_BasePage::createPageLayout()
{
    auto* contentWidget = new QWidget(this);
    contentWidget->setObjectName("ElaExampleContent");
    contentWidget->setStyleSheet("#ElaExampleContent{background-color:transparent;}");
    contentWidget->setWindowTitle(windowTitle());
    auto* mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setContentsMargins(0, 0, 2, 28);
    mainLayout->setSpacing(24);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    addCentralWidget(contentWidget, true, false, 0);
    return mainLayout;
}

QVBoxLayout* T_BasePage::addExampleSection(QVBoxLayout* pageLayout, const QString& title, const QString& description)
{
    auto* sectionWidget = new QWidget(this);
    sectionWidget->setObjectName("ElaExampleSection");
    sectionWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    sectionWidget->setStyleSheet("#ElaExampleSection{background-color:transparent;}");
    auto* titleText = new ElaText(title, this);
    titleText->setTextPixelSize(eApp->getFontPixelSize() + 5);
    titleText->setIsBold(true);
    auto* mainLayout = new QVBoxLayout(sectionWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(titleText);
    if (!description.isEmpty())
    {
        auto* descriptionText = new ElaText(description, this);
        descriptionText->setTextPixelSize(eApp->getFontPixelSize());
        descriptionText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        mainLayout->addWidget(descriptionText);
    }
    auto* previewArea = new ElaScrollPageArea(this);
    previewArea->setMinimumHeight(0);
    previewArea->setMaximumHeight(QWIDGETSIZE_MAX);
    previewArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    previewArea->setBorderRadius(8);
    previewArea->setProperty("ElaExampleCard", true);
    auto* previewLayout = new QVBoxLayout(previewArea);
    previewLayout->setContentsMargins(20, 20, 20, 20);
    previewLayout->setSpacing(16);
    mainLayout->addWidget(previewArea);
    pageLayout->addWidget(sectionWidget);
    return previewLayout;
}

QWidget* T_BasePage::createLabeledControl(const QString& title, QWidget* control)
{
    auto* fieldWidget = new QWidget(this);
    fieldWidget->setObjectName("ElaExampleField");
    fieldWidget->setStyleSheet("#ElaExampleField{background-color:transparent;}");
    auto* labelText = new ElaText(title, this);
    labelText->setTextPixelSize(eApp->getFontPixelSize());
    labelText->setBuddy(control);
    control->setAccessibleName(title);
    auto* mainLayout = new QVBoxLayout(fieldWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(labelText);
    mainLayout->addWidget(control, 0, Qt::AlignLeft);
    return fieldWidget;
}

QWidget* T_BasePage::createSettingRow(const QString& title, const QString& description, QWidget* control)
{
    auto* rowWidget = new QWidget(this);
    rowWidget->setObjectName("ElaExampleSettingRow");
    rowWidget->setStyleSheet("#ElaExampleSettingRow{background-color:transparent;}");
    auto* titleText = new ElaText(title, this);
    titleText->setTextPixelSize(eApp->getFontPixelSize() + 1);
    auto* descriptionText = new ElaText(description, this);
    descriptionText->setTextPixelSize(eApp->getFontPixelSize() - 1);
    descriptionText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    auto* textLayout = new QVBoxLayout();
    textLayout->setSpacing(4);
    textLayout->addWidget(titleText);
    textLayout->addWidget(descriptionText);
    auto* mainLayout = new QHBoxLayout(rowWidget);
    mainLayout->setContentsMargins(0, 4, 0, 4);
    mainLayout->setSpacing(20);
    mainLayout->addLayout(textLayout, 1);
    mainLayout->addWidget(control, 0, Qt::AlignVCenter);
    control->setAccessibleName(title);
    return rowWidget;
}

void T_BasePage::setPreviewPlaceholder(ElaImageCard* previewCard, const QString& description)
{
    previewCard->setMinimumWidth(0);
    previewCard->setFixedHeight(678);
    previewCard->setIsPreserveAspectCrop(true);
    auto* placeholderText = new ElaText(description, previewCard);
    placeholderText->setTextPixelSize(eApp->getFontPixelSize() + 2);
    placeholderText->setAlignment(Qt::AlignCenter);
    auto* mainLayout = new QVBoxLayout(previewCard);
    mainLayout->addWidget(placeholderText);
    connect(previewCard, &ElaImageCard::pCardImageChanged, placeholderText, [previewCard, placeholderText]() {
        placeholderText->setVisible(previewCard->getCardImage().isNull());
    });
}
