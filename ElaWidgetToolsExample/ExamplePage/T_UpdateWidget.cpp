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
    ElaText* updateTitle = new ElaText("2025-11-21更新", 15, this);
    ElaText* update1 = new ElaText("1、ElaWindow新增绘制模式切换、堆栈内部自定义窗口支持", 13, this);
    ElaText* update2 = new ElaText("2、ElaNavigationBar导航栏布局优化", 13, this);
    ElaText* update3 = new ElaText("3、ElaFlowLayout流式布局性能及效果优化", 13, this);
    ElaText* update4 = new ElaText("4、ElaToolButton新增图标旋转支持", 13, this);
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
