#include "T_Card.h"

#include <QVBoxLayout>

#include "ElaPromotionCard.h"
#include "ElaPromotionView.h"

T_Card::T_Card(QWidget* parent)
    : ElaScrollPage(parent)
{
    _promotionCard = new ElaPromotionCard(this);
    _promotionCard->setFixedSize(600, 300);
    _promotionCard->setCardPixmap(QPixmap(":/Resource/Image/Card/miku.png"));
    _promotionCard->setCardTitle("MiKu");
    _promotionCard->setPromotionTitle("SONG~");
    _promotionCard->setTitle("STYX HELIX");
    _promotionCard->setSubTitle("Never close your eyes, Searching for a true fate");

    _promotionView = new ElaPromotionView(this);

    ElaPromotionCard* exampleCard1 = new ElaPromotionCard(this);
    exampleCard1->setCardPixmap(QPixmap(":/Resource/Image/Card/miku.png"));
    exampleCard1->setCardTitle("MiKu");
    exampleCard1->setPromotionTitle("SONG~");
    exampleCard1->setTitle("STYX HELIX");
    exampleCard1->setSubTitle("Never close your eyes, Searching for a true fate");

    ElaPromotionCard* exampleCard2 = new ElaPromotionCard(this);
    exampleCard2->setCardPixmap(QPixmap(":/Resource/Image/Card/beach.png"));
    exampleCard2->setCardTitle("Beach");
    exampleCard2->setPromotionTitle("SONG~");
    exampleCard2->setTitle("STYX HELIX");
    exampleCard2->setSubTitle("Never close your eyes, Searching for a true fate");

    ElaPromotionCard* exampleCard3 = new ElaPromotionCard(this);
    exampleCard3->setCardPixmap(QPixmap(":/Resource/Image/Card/dream.png"));
    exampleCard3->setCardTitle("Dream");
    exampleCard3->setPromotionTitle("SONG~");
    exampleCard3->setTitle("STYX HELIX");
    exampleCard3->setSubTitle("Never close your eyes, Searching for a true fate");

    ElaPromotionCard* exampleCard4 = new ElaPromotionCard(this);
    exampleCard4->setCardPixmap(QPixmap(":/Resource/Image/Card/classroom.png"));
    exampleCard4->setCardTitle("Classroom");
    exampleCard4->setPromotionTitle("SONG~");
    exampleCard4->setTitle("STYX HELIX");
    exampleCard4->setSubTitle("Never close your eyes, Searching for a true fate");

    _promotionView->appendPromotionCard(exampleCard1);
    _promotionView->appendPromotionCard(exampleCard2);
    _promotionView->appendPromotionCard(exampleCard3);
    _promotionView->appendPromotionCard(exampleCard4);
    _promotionView->setIsAutoScroll(true);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaCard");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addWidget(_promotionCard);
    centerLayout->addSpacing(20);
    centerLayout->addWidget(_promotionView);
    centerLayout->addSpacing(100);
    centerLayout->addStretch();
    addCentralWidget(centralWidget, true, true, 0);
}

T_Card::~T_Card()
{
}
