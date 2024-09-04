#include "T_Graphics.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaGraphicsItem.h"
#include "ElaGraphicsScene.h"
#include "ElaGraphicsView.h"
T_Graphics::T_Graphics(QWidget* parent)
    : T_BasePage(parent)
{
    // 顶部元素
    QVBoxLayout* topLayout = createTopLayout("图形视图框架被放置于此，可在此界面体验其效果，按住Ctrl进行缩放，按住Shitf进行连接");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaGraphics");

    // GraphicsView
    ElaGraphicsScene* scene = new ElaGraphicsScene(this);
    scene->setSceneRect(0, 0, 1500, 1500);
    ElaGraphicsItem* item1 = new ElaGraphicsItem();
    item1->setWidth(100);
    item1->setHeight(100);
    item1->setMaxLinkPortCount(100);
    item1->setMaxLinkPortCount(1);
    ElaGraphicsItem* item2 = new ElaGraphicsItem();
    item2->setWidth(100);
    item2->setHeight(100);
    scene->addItem(item1);
    scene->addItem(item2);
    ElaGraphicsView* view = new ElaGraphicsView(scene);
    view->setScene(scene);
    view->setFixedHeight(600);

    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addLayout(topLayout);
    centerVLayout->addSpacing(5);
    centerVLayout->addWidget(view);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_Graphics::~T_Graphics()
{
}
