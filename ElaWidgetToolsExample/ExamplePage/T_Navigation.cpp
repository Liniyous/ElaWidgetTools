#include "T_Navigation.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "ElaBreadcrumbBar.h"
#include "ElaPivot.h"
#include "ElaPushButton.h"
#include "ElaScrollPageArea.h"
#include "ElaTabWidget.h"
#include "ElaText.h"
T_Navigation::T_Navigation(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaNavigation");

    // 顶部元素
    createCustomWidget("一些导航组件被放置于此，可在此界面体验其效果并按需添加进项目中");

    //ElaBreadcrumbBar
    ElaText* breadcrumbBarText = new ElaText("ElaBreadcrumbBar", this);
    breadcrumbBarText->setTextPixelSize(18);
    _breadcrumbBar = new ElaBreadcrumbBar(this);
    QStringList breadcrumbBarList;
    for (int i = 0; i < 20; i++)
    {
        breadcrumbBarList << QString("Item%1").arg(i + 1);
    }
    _breadcrumbBar->setBreadcrumbList(breadcrumbBarList);

    ElaPushButton* resetButton = new ElaPushButton("还原", this);
    resetButton->setFixedSize(60, 32);
    connect(resetButton, &ElaPushButton::clicked, this, [=]() {
        _breadcrumbBar->setBreadcrumbList(breadcrumbBarList);
    });

    QHBoxLayout* breadcrumbBarTextLayout = new QHBoxLayout();
    breadcrumbBarTextLayout->addWidget(breadcrumbBarText);
    breadcrumbBarTextLayout->addSpacing(15);
    breadcrumbBarTextLayout->addWidget(resetButton);
    breadcrumbBarTextLayout->addStretch();

    ElaScrollPageArea* breadcrumbBarArea = new ElaScrollPageArea(this);
    QVBoxLayout* breadcrumbBarLayout = new QVBoxLayout(breadcrumbBarArea);
    breadcrumbBarLayout->addWidget(_breadcrumbBar);

    // ElaPivot
    ElaText* pivotText = new ElaText("ElaPivot", this);
    pivotText->setTextPixelSize(18);
    _pivot = new ElaPivot(this);
    _pivot->setPivotSpacing(8);
    _pivot->setMarkWidth(75);
    _pivot->appendPivot("本地歌曲");
    _pivot->appendPivot("下载歌曲");
    _pivot->appendPivot("下载视频");
    _pivot->appendPivot("正在下载");
    _pivot->appendPivot("本地歌曲");
    _pivot->appendPivot("下载歌曲");
    _pivot->appendPivot("下载视频");
    _pivot->appendPivot("正在下载");
    _pivot->appendPivot("本地歌曲");
    _pivot->appendPivot("下载歌曲");
    _pivot->appendPivot("下载视频");
    _pivot->appendPivot("正在下载");
    _pivot->setCurrentIndex(0);

    ElaScrollPageArea* pivotArea = new ElaScrollPageArea(this);
    QVBoxLayout* pivotLayout = new QVBoxLayout(pivotArea);
    pivotLayout->addWidget(_pivot);

    // ElaTabWidget
    ElaText* tabWidgetText = new ElaText("ElaTabWidget", this);
    tabWidgetText->setTextPixelSize(18);
    _tabWidget = new ElaTabWidget(this);
    _tabWidget->setFixedHeight(500);
    _tabWidget->setIsTabTransparent(true);
    ElaText* page1 = new ElaText("新标签页1", this);
    page1->setTextPixelSize(32);
    page1->setAlignment(Qt::AlignCenter);
    ElaText* page2 = new ElaText("新标签页2", this);
    page2->setTextPixelSize(32);
    page2->setAlignment(Qt::AlignCenter);
    ElaText* page3 = new ElaText("新标签页3", this);
    page3->setTextPixelSize(32);
    page3->setAlignment(Qt::AlignCenter);
    ElaText* page4 = new ElaText("新标签页4", this);
    page4->setTextPixelSize(32);
    page4->setAlignment(Qt::AlignCenter);
    _tabWidget->addTab(page1, QIcon(":/Resource/Image/Cirno.jpg"), "新标签页1");
    _tabWidget->addTab(page2, "新标签页2");
    _tabWidget->addTab(page3, "新标签页3");
    _tabWidget->addTab(page4, "新标签页4");
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaNavigation");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addLayout(breadcrumbBarTextLayout);
    centerVLayout->addSpacing(10);
    centerVLayout->addWidget(breadcrumbBarArea);
    centerVLayout->addSpacing(15);
    centerVLayout->addWidget(pivotText);
    centerVLayout->addSpacing(10);
    centerVLayout->addWidget(pivotArea);
    centerVLayout->addSpacing(15);
    centerVLayout->addWidget(tabWidgetText);
    centerVLayout->addSpacing(10);
    centerVLayout->addWidget(_tabWidget);
    addCentralWidget(centralWidget, true, false, 0);
}

T_Navigation::~T_Navigation()
{
}
