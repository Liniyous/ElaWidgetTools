#include "T_UpdateWidget.h"

#include <QVBoxLayout>

#include "ElaText.h"
T_UpdateWidget::T_UpdateWidget(QWidget* parent)
    : QWidget{parent}
{
    setMinimumSize(200, 260);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    mainLayout->setContentsMargins(5, 10, 5, 5);
    mainLayout->setSpacing(4);
    ElaText* updateTitle = new ElaText("2025-8-21更新", 15, this);
    ElaText* update1 = new ElaText("1、兼容Win7原生无边框、Win10外部无边框", 13, this);
    ElaText* update2 = new ElaText("2、优化ElaDrawer抽屉组件展开收起效果", 13, this);
    ElaText* update3 = new ElaText("3、新增主要堆栈功能和堆栈切换动画支持", 13, this);
    ElaText* update4 = new ElaText("4、Linux部分组件体验优化", 13, this);
    ElaText* update5 = new ElaText("5、QQ交流群: 850243692", 13, this);
    update1->setIsWrapAnywhere(true);
    update2->setIsWrapAnywhere(true);
    update3->setIsWrapAnywhere(true);
    update4->setIsWrapAnywhere(true);

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
