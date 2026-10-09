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
    ElaText* updateTitle = new ElaText("2026-10-09 更新汇总", 15, this);
    ElaText* update1 = new ElaText("1、增强 Ribbon 与 Pivot 联动，完善动作状态同步。", 13, this);
    ElaText* update2 = new ElaText("2、标签页支持平滑滚动与定位，改进拖拽及浮动页面管理。", 13, this);
    ElaText* update3 = new ElaText("3、优化 DXGI 采集稳定性，完善图像快照与资源释放。", 13, this);
    ElaText* update4 = new ElaText("4、完善全局字号适配，优化控件尺寸及交互动画。", 13, this);
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
