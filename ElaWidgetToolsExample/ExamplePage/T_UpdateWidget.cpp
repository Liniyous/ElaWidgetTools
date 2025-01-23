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
    ElaText* updateTitle = new ElaText("2025-1-23更新", 15, this);
    ElaText* update1 = new ElaText("1、大幅优化了项目的编译体验~", 13, this);
    ElaText* update2 = new ElaText("2、ElaPacketIO库正式发布！该库为基于包定义的轻量级RPC库，基于波音的Afsim拆离派生而来；详情请进群了解~", 13, this);
    ElaText* update3 = new ElaText("3、新增UDP分布式视频流传输例子，使用ElaPacketIO~", 13, this);
    ElaText* update4 = new ElaText("4、QQ交流群: 850243692", 13, this);
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
