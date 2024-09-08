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
    ElaText* updateTitle = new ElaText("2024-9-8更新", 15, this);
    ElaText* update1 = new ElaText("1、新增ElaColorDialog 颜色选择器", 13, this);
    ElaText* update2 = new ElaText("2、ElaNavigationBar新增多窗口操作", 13, this);
    ElaText* update3 = new ElaText("3、ElaWindowMica模式补正", 13, this);
    ElaText* update4 = new ElaText("4、新增ElaScrollBar浮动模式", 13, this);
    ElaText* update5 = new ElaText("5、ElaAppBar固定模式优化，以及一系列组件细节优化", 13, this);
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
