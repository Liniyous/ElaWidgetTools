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
    ElaText* updateTitle = new ElaText("2025-5-25更新", 15, this);
    ElaText* update1 = new ElaText("1、适配原生Mica、Mica-Alt、Acrylic、Dwm-Blur模式", 13, this);
    ElaText* update2 = new ElaText("2、优化列表组件的视觉效果", 13, this);
    ElaText* update3 = new ElaText("3、修正ElaComboBox可编辑模式下不正确的显示状态", 13, this);
    ElaText* update4 = new ElaText("4、新增ElaProgressRing环形进度条组件", 13, this);
    ElaText* update5 = new ElaText("4、QQ交流群: 850243692", 13, this);
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
