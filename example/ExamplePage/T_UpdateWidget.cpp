#include "T_UpdateWidget.h"

#include <QVBoxLayout>

#include "ElaText.h"
T_UpdateWidget::T_UpdateWidget(QWidget* parent)
    : QWidget{parent}
{
    setMinimumSize(200, 245);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    mainLayout->setContentsMargins(5, 10, 5, 5);
    mainLayout->setSpacing(5);
    ElaText* updateTitle = new ElaText("2024-7-21更新", 15, this);
    ElaText* update1 = new ElaText("1、ElaWindow已升格为QMainWindow", 13, this);
    ElaText* update2 = new ElaText("2、ElaMenu ElaToolButton ElaComboBox动画重做", 13, this);
    ElaText* update3 = new ElaText("3、新增ElaStatusBar ElaMenuBar ElaToolBar ElaDockWidget", 13, this);
    ElaText* update4 = new ElaText("4、重做了ElaComboBox动画", 13, this);
    ElaText* update5 = new ElaText("5、黑色主题整体风格优化", 13, this);
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
