#include "T_TreeView.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QVBoxLayout>

#include "ElaPushButton.h"
#include "ElaScrollBar.h"
#include "ElaSlider.h"
#include "ElaText.h"
#include "ElaTreeView.h"
#include "T_TreeViewModel.h"
T_TreeView::T_TreeView(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaTreeView");

    // 顶部元素
    createCustomWidget("树型视图被放置于此，可在此界面体验其效果并按需添加进项目中");

    // ElaTreeView
    T_TreeViewModel* treeModel = new T_TreeViewModel(this);
    QHBoxLayout* treeLayout = new QHBoxLayout();
    treeLayout->setContentsMargins(0, 0, 10, 0);
    QWidget* treeSettingWidget = new QWidget(this);
    QVBoxLayout* treeSettingWidgetLayout = new QVBoxLayout(treeSettingWidget);
    treeSettingWidgetLayout->setContentsMargins(0, 0, 0, 0);
    treeSettingWidgetLayout->setSpacing(15);

    // 数据统计
    ElaText* dataText = new ElaText(QString("树模型总数据条数：%1").arg(treeModel->getItemCount()), this);
    dataText->setTextPixelSize(15);

    // ItemHeight
    ElaText* itemHeightText = new ElaText("ItemHeight", this);
    itemHeightText->setTextPixelSize(15);
    ElaSlider* itemHeightSlider = new ElaSlider(this);
    itemHeightSlider->setRange(200, 600);
    itemHeightSlider->setValue(350);
    ElaText* itemHeightValueText = new ElaText("35", this);
    itemHeightValueText->setTextPixelSize(15);
    connect(itemHeightSlider, &ElaSlider::valueChanged, this, [=](int value) {
        itemHeightValueText->setText(QString::number(value / 10));
        _treeView->setItemHeight(value / 10);
    });
    QHBoxLayout* itemHeightLayout = new QHBoxLayout();
    itemHeightLayout->setContentsMargins(0, 0, 0, 0);
    itemHeightLayout->addWidget(itemHeightText);
    itemHeightLayout->addWidget(itemHeightSlider);
    itemHeightLayout->addWidget(itemHeightValueText);

    // HeaderMargin
    ElaText* headerMarginText = new ElaText("HeaderMargin", this);
    headerMarginText->setTextPixelSize(15);
    ElaSlider* headerMarginSlider = new ElaSlider(this);
    headerMarginSlider->setRange(0, 200);
    headerMarginSlider->setValue(50);
    ElaText* headerMarginValueText = new ElaText("5", this);
    headerMarginValueText->setTextPixelSize(15);
    connect(headerMarginSlider, &ElaSlider::valueChanged, this, [=](int value) {
        headerMarginValueText->setText(QString::number(value / 10));
        _treeView->setHeaderMargin(value / 10);
    });
    QHBoxLayout* headerMarginLayout = new QHBoxLayout();
    headerMarginLayout->setContentsMargins(0, 0, 0, 0);
    headerMarginLayout->addWidget(headerMarginText);
    headerMarginLayout->addWidget(headerMarginSlider);
    headerMarginLayout->addWidget(headerMarginValueText);

    // Indentation
    ElaText* indentationText = new ElaText("Indentation", this);
    indentationText->setTextPixelSize(15);
    ElaSlider* indentationSlider = new ElaSlider(this);
    indentationSlider->setRange(200, 1000);
    indentationSlider->setValue(200);
    ElaText* indentationValueText = new ElaText("20", this);
    indentationValueText->setTextPixelSize(15);
    connect(indentationSlider, &ElaSlider::valueChanged, this, [=](int value) {
        indentationValueText->setText(QString::number(value / 10));
        _treeView->setIndentation(value / 10);
    });
    QHBoxLayout* indentationLayout = new QHBoxLayout();
    indentationLayout->setContentsMargins(0, 0, 0, 0);
    indentationLayout->addWidget(indentationText);
    indentationLayout->addWidget(indentationSlider);
    indentationLayout->addWidget(indentationValueText);

    //展开全部
    QHBoxLayout* expandCollapseLayout = new QHBoxLayout();
    expandCollapseLayout->setContentsMargins(0, 0, 0, 0);
    ElaPushButton* expandButton = new ElaPushButton("展开全部", this);
    expandButton->setFixedWidth(80);
    connect(expandButton, &ElaPushButton::clicked, this, [=]() {
        _treeView->expandAll();
    });

    //收起全部
    ElaPushButton* collapseButton = new ElaPushButton("收起全部", this);
    collapseButton->setFixedWidth(80);
    connect(collapseButton, &ElaPushButton::clicked, this, [=]() {
        _treeView->collapseAll();
    });
    expandCollapseLayout->addWidget(expandButton);
    expandCollapseLayout->addWidget(collapseButton);
    expandCollapseLayout->addStretch();

    treeSettingWidgetLayout->addWidget(dataText);
    treeSettingWidgetLayout->addLayout(itemHeightLayout);
    treeSettingWidgetLayout->addLayout(headerMarginLayout);
    treeSettingWidgetLayout->addLayout(indentationLayout);
    treeSettingWidgetLayout->addLayout(expandCollapseLayout);
    treeSettingWidgetLayout->addStretch();

    // TreeView
    ElaText* treeText = new ElaText("ElaTreeView", this);
    treeText->setTextPixelSize(18);
    _treeView = new ElaTreeView(this);
    ElaScrollBar* treeViewFloatScrollBar = new ElaScrollBar(_treeView->verticalScrollBar(), _treeView);
    treeViewFloatScrollBar->setIsAnimation(true);
    QFont headerFont = _treeView->header()->font();
    headerFont.setPixelSize(16);
    _treeView->header()->setFont(headerFont);
    _treeView->setFixedHeight(450);
    _treeView->setModel(treeModel);
    treeLayout->addWidget(treeSettingWidget);
    treeLayout->addWidget(_treeView);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaView");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(treeText);
    centerVLayout->addSpacing(10);
    centerVLayout->addLayout(treeLayout);
    addCentralWidget(centralWidget, true, false, 0);
}

T_TreeView::~T_TreeView()
{
}
