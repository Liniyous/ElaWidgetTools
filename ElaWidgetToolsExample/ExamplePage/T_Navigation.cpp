#include "T_Navigation.h"

#include "ElaBreadcrumbBar.h"
#include "ElaFlowLayout.h"
#include "ElaMenu.h"
#include "ElaPivot.h"
#include "ElaPushButton.h"
#include "ElaTabWidget.h"
#include "ElaText.h"

#include <QVBoxLayout>

T_Navigation::T_Navigation(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("导航与标签");
    createCustomWidget("通过路径、分类与标签组织内容，点击控件查看当前导航位置。", "导航与标签");
    auto* mainLayout = createPageLayout();
    auto* breadcrumbSection = addExampleSection(mainLayout, "路径导航 · ElaBreadcrumbBar", "点击上级路径返回，使用还原按钮重新展示完整路径。");
    const QStringList path{"工作空间", "设计系统", "组件库", "导航组件"};
    _breadcrumbBar = new ElaBreadcrumbBar(this);
    _breadcrumbBar->setBreadcrumbList(path);
    auto* resetButton = new ElaPushButton("还原路径", this);
    resetButton->setFixedWidth(100);
    auto* breadcrumbResultText = new ElaText("当前位置：导航组件", this);
    connect(resetButton, &ElaPushButton::clicked, this, [this, path, breadcrumbResultText]() {
        _breadcrumbBar->setBreadcrumbList(path);
        breadcrumbResultText->setText("当前位置：导航组件");
    });
    connect(_breadcrumbBar, &ElaBreadcrumbBar::breadcrumbClicked, this, [breadcrumbResultText](const QString& value, const QStringList&) {
        breadcrumbResultText->setText("当前位置：" + value);
    });
    breadcrumbSection->addWidget(_breadcrumbBar);
    auto* breadcrumbActionLayout = new QHBoxLayout();
    breadcrumbActionLayout->addWidget(breadcrumbResultText, 1);
    breadcrumbActionLayout->addWidget(resetButton);
    breadcrumbSection->addLayout(breadcrumbActionLayout);

    auto* pivotSection = addExampleSection(mainLayout, "分类切换 · ElaPivot", "点击分类，观察选中标记和内容的联动。");
    _pivot = new ElaPivot(this);
    _pivot->setPivotSpacing(18);
    _pivot->setMarkWidth(28);
    const QStringList categories{"概览", "设计", "开发", "发布"};
    for (const QString& title: categories)
    {
        _pivot->appendPivot(title);
    }
    _pivot->setCurrentIndex(0);
    auto* pivotResultText = new ElaText("概览：查看项目进度与最近活动。", this);
    const QStringList descriptions{"概览：查看项目进度与最近活动。", "设计：整理视觉规范和交互说明。", "开发：浏览组件实现与接口。", "发布：检查版本信息与交付清单。"};
    connect(_pivot, &ElaPivot::pivotClicked, this, [pivotResultText, descriptions](int index) {
        pivotResultText->setText(descriptions.value(index));
    });
    pivotSection->addWidget(_pivot);
    pivotSection->addWidget(pivotResultText);

    auto* tabSection = addExampleSection(mainLayout, "多文档标签 · ElaTabWidget", "切换、拖动或关闭标签；右键可固定标签，也可以新增一个页面。");
    auto* addButton = new ElaPushButton("新增标签", this);
    _tabWidget = new ElaTabWidget(this);
    _tabWidget->setFixedHeight(280);
    _tabWidget->setIsTabTransparent(true);
    _tabWidget->setIsTabWidthAdaptive(true);
    for (const QString& title: {QString("项目概览"), QString("界面设计"), QString("开发记录")})
    {
        auto* pageText = new ElaText(title + "\n\n这是一个可以独立切换的文档页面", this);
        pageText->setTextPixelSize(18);
        pageText->setAlignment(Qt::AlignCenter);
        _tabWidget->addTab(pageText, title);
    }
    connect(addButton, &ElaPushButton::clicked, this, [this]() {
        const QString title = QString("新文档 %1").arg(_tabWidget->count() + 1);
        auto* pageText = new ElaText(title, this);
        pageText->setTextPixelSize(18);
        pageText->setAlignment(Qt::AlignCenter);
        _tabWidget->setCurrentIndex(_tabWidget->addTab(pageText, title));
    });
    connect(_tabWidget, &ElaTabWidget::tabCloseRequested, this, [this](int index) {
        QWidget* page = _tabWidget->widget(index);
        _tabWidget->removeTab(index);
        page->deleteLater();
    });
    connect(_tabWidget, &ElaTabWidget::tabContextMenuRequested, this, [this](int index, const QPoint& position) {
        ElaMenu menu(this);
        const bool isPinned = _tabWidget->getIsTabPinned(index);
        connect(menu.addAction(isPinned ? "取消固定" : "固定标签"), &QAction::triggered, this, [this, index, isPinned]() {
            _tabWidget->setIsTabPinned(index, !isPinned);
        });
        menu.exec(position);
    });
    tabSection->addWidget(addButton, 0, Qt::AlignLeft);
    tabSection->addWidget(_tabWidget);
}

T_Navigation::~T_Navigation()
{
}
