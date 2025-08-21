#include "T_About.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QVBoxLayout>

#include "ElaImageCard.h"
#include "ElaText.h"
T_About::T_About(QWidget* parent)
    : ElaDialog(parent)
{
    setWindowTitle("关于..");
    setWindowIcon(QIcon(":/include/Image/Moon.jpg"));
    this->setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);
    ElaImageCard* pixCard = new ElaImageCard(this);
    pixCard->setFixedSize(60, 60);
    pixCard->setIsPreserveAspectCrop(false);
    pixCard->setCardImage(QImage(":/include/Image/Moon.jpg"));

    QVBoxLayout* pixCardLayout = new QVBoxLayout();
    pixCardLayout->addWidget(pixCard);
    pixCardLayout->addStretch();

    ElaText* versionText = new ElaText("ElaWidgetTools-LK-2024", this);
    QFont versionTextFont = versionText->font();
    versionTextFont.setWeight(QFont::Bold);
    versionText->setFont(versionTextFont);
    versionText->setWordWrap(false);
    versionText->setTextPixelSize(18);

    ElaText* licenseText = new ElaText("MIT授权协议", this);
    licenseText->setWordWrap(false);
    licenseText->setTextPixelSize(14);
    ElaText* supportText = new ElaText("Windows支持版本: QT5.12以上\nLinux支持版本: Qt5.14以上", this);
    supportText->setWordWrap(false);
    supportText->setTextPixelSize(14);
    ElaText* contactText = new ElaText("作者: 80985@qq.com\n交流群: 850243692(QQ)", this);
    contactText->setWordWrap(false);
    contactText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    contactText->setTextPixelSize(14);
    ElaText* helperText = new ElaText("用户手册及API文档付费提供\n提供额外的专业技术支持", this);
    helperText->setWordWrap(false);
    helperText->setTextPixelSize(14);
    ElaText* copyrightText = new ElaText("版权所有 © 2024 Liniyous", this);
    copyrightText->setWordWrap(false);
    copyrightText->setTextPixelSize(14);

    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(15);
    textLayout->addWidget(versionText);
    textLayout->addWidget(licenseText);
    textLayout->addWidget(supportText);
    textLayout->addWidget(contactText);
    textLayout->addWidget(helperText);
    textLayout->addWidget(copyrightText);
    textLayout->addStretch();

    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addSpacing(30);
    contentLayout->addLayout(pixCardLayout);
    contentLayout->addSpacing(30);
    contentLayout->addLayout(textLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 25, 0, 0);
    mainLayout->addLayout(contentLayout);
}

T_About::~T_About()
{
}
