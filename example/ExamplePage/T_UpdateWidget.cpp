#include "T_UpdateWidget.h"

#include <QVBoxLayout>

#include "ElaText.h"
T_UpdateWidget::T_UpdateWidget(QWidget* parent)
    : QWidget{parent}
{
    setMinimumSize(200, 280);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    mainLayout->setContentsMargins(5, 10, 5, 5);
    mainLayout->setSpacing(4);
    ElaText* updateTitle = new ElaText("2024-8-04更新", 15, this);
    ElaText* update1 = new ElaText("1、新增ElaPopularCard 热门卡片", 13, this);
    ElaText* update2 = new ElaText("2、ElaToolBar ElaToolButton外观及功能优化", 13, this);
    ElaText* update3 = new ElaText("3、ElaWindow布局和整体外观优化", 13, this);
    ElaText* update4 = new ElaText("4、ElaCheckBox，新增三态支持", 13, this);
    ElaText* update5 = new ElaText("5、ElaAppBar功能优化，新增CustomWidget自定义窗口区域", 13, this);
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
