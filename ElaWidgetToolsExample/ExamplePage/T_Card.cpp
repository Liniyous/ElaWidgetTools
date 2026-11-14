#include "T_Card.h"

#include "ElaAcrylicUrlCard.h"
#include "ElaCheckBox.h"
#include "ElaFlowLayout.h"
#include "ElaInteractiveCard.h"
#include "ElaLCDNumber.h"
#include "ElaPromotionCard.h"
#include "ElaPromotionView.h"
#include "ElaText.h"

#include <QVBoxLayout>

T_Card::T_Card(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("卡片展示");
    createCustomWidget("组合图片、标题和操作入口，体验卡片悬停、轮播与数字显示。", "卡片与内容展示");
    auto* mainLayout = createPageLayout();
    auto* promotionSection = addExampleSection(mainLayout, "推广卡片 · ElaPromotionCard", "移动指针或点击卡片，观察图片上的光效。");
    _promotionCard = new ElaPromotionCard(this);
    _promotionCard->setMinimumWidth(0);
    _promotionCard->setFixedHeight(260);
    _promotionCard->setCardPixmap(QPixmap(":/Resource/Image/Card/miku.png"));
    _promotionCard->setCardTitle("Ela 组件展示");
    _promotionCard->setPromotionTitle("精选内容");
    _promotionCard->setTitle("发现更好的界面");
    _promotionCard->setSubTitle("图片与文字组成内容入口");
    promotionSection->addWidget(_promotionCard);

    auto* linkSection = addExampleSection(mainLayout, "链接与交互卡片", "ElaAcrylicUrlCard 用于链接入口，ElaInteractiveCard 用于可点击的内容项。");
    auto* linkLayout = new ElaFlowLayout(0, 24, 16);
    auto* repositoryCard = new ElaAcrylicUrlCard(this);
    repositoryCard->setFixedSize(210, 210);
    repositoryCard->setCardPixmap(QPixmap(":/Resource/Image/github.png"));
    repositoryCard->setTitle("项目源码");
    repositoryCard->setSubTitle("浏览 ElaWidgetTools 的组件与示例");
    repositoryCard->setUrl("https://github.com/Liniyous/ElaWidgetTools");
    auto* projectCard = new ElaInteractiveCard(this);
    projectCard->setFixedSize(300, 100);
    projectCard->setCardPixmap(QPixmap(":/Resource/Image/Cirno.jpg"));
    projectCard->setTitle("示例项目");
    projectCard->setSubTitle("点击选择此项目");
    auto* cardResultText = new ElaText("尚未选择项目", this);
    connect(projectCard, &ElaInteractiveCard::clicked, this, [cardResultText]() {
        cardResultText->setText("已选择：示例项目");
    });
    linkLayout->addWidget(repositoryCard);
    linkLayout->addWidget(createLabeledControl("内容入口", projectCard));
    linkSection->addLayout(linkLayout);
    linkSection->addWidget(cardResultText);

    auto* carouselSection = addExampleSection(mainLayout, "卡片轮播 · ElaPromotionView", "点击两侧卡片或滚动鼠标滚轮切换，也可以启用自动轮播。");
    _promotionView = new ElaPromotionView(this);
    _promotionView->setFixedHeight(260);
    const QStringList images{"miku", "beach", "dream", "classroom"};
    const QStringList titles{"灵感画廊", "海边时光", "梦境色彩", "学习空间"};
    for (int index = 0; index < images.count(); ++index)
    {
        auto* card = new ElaPromotionCard(this);
        card->setCardPixmap(QPixmap(QString(":/Resource/Image/Card/%1.png").arg(images[index])));
        card->setCardTitle(QString("精选 %1").arg(index + 1));
        card->setTitle(titles[index]);
        card->setSubTitle("点击切换当前卡片");
        card->setTitlePixelSize(22);
        _promotionView->appendPromotionCard(card);
    }
    _promotionView->setCardExpandWidth(360);
    _promotionView->setCardCollapseWidth(170);
    auto* autoScrollCheckBox = new ElaCheckBox("自动轮播", this);
    connect(autoScrollCheckBox, &ElaCheckBox::toggled, _promotionView, &ElaPromotionView::setIsAutoScroll);
    carouselSection->addWidget(autoScrollCheckBox);
    carouselSection->addWidget(_promotionView);

    auto* clockSection = addExampleSection(mainLayout, "数字显示 · ElaLCDNumber", "使用自动时钟显示当前时间，可切换为透明背景。");
    _lcdNumber = new ElaLCDNumber(this);
    _lcdNumber->setFixedSize(300, 86);
    _lcdNumber->setIsUseAutoClock(true);
    _lcdNumber->setAutoClockFormat("hh:mm:ss");
    auto* transparentCheckBox = new ElaCheckBox("透明背景", this);
    connect(transparentCheckBox, &ElaCheckBox::toggled, _lcdNumber, &ElaLCDNumber::setIsTransparent);
    clockSection->addWidget(_lcdNumber, 0, Qt::AlignLeft);
    clockSection->addWidget(transparentCheckBox);
}

T_Card::~T_Card()
{
}
