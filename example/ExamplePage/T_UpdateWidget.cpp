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
    ElaText* updateTitle = new ElaText("2024-11-17更新", 15, this);
    ElaText* update1 = new ElaText("1、新增ElaKeyBinder单键位绑定组件，支持鼠标按键", 13, this);
    ElaText* update2 = new ElaText("2、ElaContentDialog中间按钮按下行为优化", 13, this);
    ElaText* update3 = new ElaText("3、ElaWindow导航模式切换相关问题修正", 13, this);
    ElaText* update4 = new ElaText("4、CMAKE编译配置和体验优化", 13, this);
    update1->setIsWrapAnywhere(true);
    update2->setIsWrapAnywhere(true);
    update3->setIsWrapAnywhere(true);
    update4->setIsWrapAnywhere(true);

    mainLayout->addWidget(updateTitle);
    mainLayout->addWidget(update1);
    mainLayout->addWidget(update2);
    mainLayout->addWidget(update3);
    mainLayout->addWidget(update4);
    mainLayout->addStretch();
}

T_UpdateWidget::~T_UpdateWidget()
{
}
