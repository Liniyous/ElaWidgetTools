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
    ElaText* updateTitle = new ElaText("2024-9-1更新", 15, this);
    ElaText* update1 = new ElaText("1、新增ElaPivot 轴转导航", 13, this);
    ElaText* update2 = new ElaText("2、ElaNavigationBar整体重构", 13, this);
    ElaText* update3 = new ElaText("3、ElaWindow新增Mica模式", 13, this);
    ElaText* update4 = new ElaText("4、ElaIcon界面搜索框灵敏问题修正", 13, this);
    ElaText* update5 = new ElaText("5、ElaIconButton新增图片模式, 多个组件细节风格优化", 13, this);
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
