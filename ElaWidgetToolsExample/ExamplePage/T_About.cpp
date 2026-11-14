#include "T_About.h"

#include "ElaImageCard.h"
#include "ElaPushButton.h"
#include "ElaText.h"

#include <QDesktopServices>
#include <QHBoxLayout>
#include <QSysInfo>
#include <QUrl>
#include <QVBoxLayout>

T_About::T_About(QWidget* parent)
    : ElaDialog(parent)
{
    setFixedSize(460, 440);
    setWindowTitle("关于 ElaWidgetTools");
    setWindowIcon(QIcon(":/include/Image/Moon.jpg"));
    setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);
    auto* imageCard = new ElaImageCard(this);
    imageCard->setFixedSize(64, 64);
    imageCard->setIsPreserveAspectCrop(false);
    imageCard->setCardImage(QImage(":/include/Image/Moon.jpg"));
    auto* titleText = new ElaText("ElaWidgetTools", this);
    titleText->setTextPixelSize(24);
    titleText->setIsBold(true);
    auto* subtitleText = new ElaText("Fluent 风格的 Qt Widgets 通用组件库", this);
    auto* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(6);
    titleLayout->addWidget(titleText);
    titleLayout->addWidget(subtitleText);
    auto* identityLayout = new QHBoxLayout();
    identityLayout->setSpacing(20);
    identityLayout->addWidget(imageCard);
    identityLayout->addLayout(titleLayout, 1);
    auto* runtimeText = new ElaText(QString("当前运行环境\nQt %1 · %2").arg(qVersion(), QSysInfo::buildCpuArchitecture()), this);
    auto* licenseText = new ElaText("开源许可 · MIT\n可在项目中使用、修改和分发，具体条款请参阅项目许可证。", this);
    auto* contactText = new ElaText("联系作者 · 80985@qq.com\n交流社区 · 850243692（QQ）", this);
    contactText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    auto* repositoryButton = new ElaPushButton("打开项目主页", this);
    repositoryButton->setIsAccent(true);
    connect(repositoryButton, &ElaPushButton::clicked, this, []() {
        QDesktopServices::openUrl(QUrl("https://github.com/Liniyous/ElaWidgetTools"));
    });
    auto* copyrightText = new ElaText("© 2026 Liniyous", this);
    copyrightText->setTextPixelSize(12);
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 52, 28, 24);
    mainLayout->setSpacing(20);
    mainLayout->addLayout(identityLayout);
    mainLayout->addWidget(runtimeText);
    mainLayout->addWidget(licenseText);
    mainLayout->addWidget(contactText);
    mainLayout->addStretch();
    auto* actionLayout = new QHBoxLayout();
    actionLayout->addWidget(copyrightText);
    actionLayout->addStretch();
    actionLayout->addWidget(repositoryButton);
    mainLayout->addLayout(actionLayout);
}

T_About::~T_About()
{
}
