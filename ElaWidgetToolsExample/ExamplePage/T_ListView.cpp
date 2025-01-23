#include "T_ListView.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaListView.h"
#include "ElaScrollBar.h"
#include "ElaText.h"
#include "T_ListViewModel.h"
T_ListView::T_ListView(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaListView");

    // 顶部元素
    createCustomWidget("列表视图被放置于此，可在此界面体验其效果并按需添加进项目中");

    //ElaListView
    ElaText* listText = new ElaText("ElaListView", this);
    listText->setTextPixelSize(18);
    _listView = new ElaListView(this);
    _listView->setFixedHeight(450);
    // _listView->setAlternatingRowColors(true);
    _listView->setModel(new T_ListViewModel(this));
    ElaScrollBar* listViewFloatScrollBar = new ElaScrollBar(_listView->verticalScrollBar(), _listView);
    listViewFloatScrollBar->setIsAnimation(true);
    QHBoxLayout* listViewLayout = new QHBoxLayout();
    listViewLayout->setContentsMargins(0, 0, 10, 0);
    listViewLayout->addWidget(_listView);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaView");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(listText);
    centerVLayout->addSpacing(10);
    centerVLayout->addLayout(listViewLayout);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_ListView::~T_ListView()
{
}
