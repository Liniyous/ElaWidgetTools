#include "T_UpdateWidget.h"

#include "ElaText.h"

#include <QVBoxLayout>

T_UpdateWidget::T_UpdateWidget(QWidget* parent)
    : QWidget{parent}
{
    setMinimumSize(200, 260);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    mainLayout->setContentsMargins(5, 10, 5, 5);
    mainLayout->setSpacing(4);
    ElaText* updateTitle = new ElaText("2026-11-14 更新汇总", 15, this);
    ElaText* update1 = new ElaText("1、优化控件配色与焦点动画，改善主题切换及交互反馈。", 13, this);
    ElaText* update2 = new ElaText("2、完善下拉框与菜单展开效果，优化弹出定位及圆角阴影。", 13, this);
    ElaText* update3 = new ElaText("3、改进抽屉展开与收回动画，完善内容适配及标题交互。", 13, this);
    ElaText* update4 = new ElaText("4、优化日历切换与日期显示，完善列表样式及表格排序。", 13, this);
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
