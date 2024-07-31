#include "T_UpdateWidget.h"

#include <QVBoxLayout>

#include "ElaText.h"
T_UpdateWidget::T_UpdateWidget(QWidget* parent)
    : QWidget{parent}
{
    setMinimumSize(200, 255);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    mainLayout->setContentsMargins(5, 10, 5, 5);
    mainLayout->setSpacing(4);
    ElaText* updateTitle = new ElaText("2024-7-30更新", 15, this);
    ElaText* update1 = new ElaText("1、新增ElaPromotionCard 促销卡片、ElaPromotionView 促销卡片视窗", 13, this);
    ElaText* update2 = new ElaText("2、ElaToggleButton API调整", 13, this);
    ElaText* update3 = new ElaText("3、ElaWindow关闭动画回调", 13, this);
    ElaText* update4 = new ElaText("4、ElaCheckBox 新增三态支持", 13, this);
    ElaText* update5 = new ElaText("5、QRC文件拆分，库QRC只保留必要资源", 13, this);
    update1->setIsWrapAnywhere(true);
    update2->setIsWrapAnywhere(true);
    update3->setIsWrapAnywhere(true);
    update4->setIsWrapAnywhere(true);
    update5->setIsWrapAnywhere(true);

    mainLayout->addWidget(updateTitle);
    mainLayout->addWidget(update1);
    mainLayout->addWidget(update2);
    mainLayout->addWidget(update3);
    mainLayout->addWidget(update4);
    mainLayout->addWidget(update5);
    mainLayout->addStretch();
}

T_UpdateWidget::~T_UpdateWidget()
{
}
