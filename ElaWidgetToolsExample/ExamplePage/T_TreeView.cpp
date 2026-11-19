#include "T_TreeView.h"

#include "ElaFlowLayout.h"
#include "ElaPushButton.h"
#include "ElaSpinBox.h"
#include "ElaText.h"
#include "ElaTreeView.h"
#include "T_TreeViewModel.h"

#include <QHeaderView>
#include <QVBoxLayout>

T_TreeView::T_TreeView(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("树形视图");
    createCustomWidget("浏览多层项目资源，调节行高、缩进和表头间距，观察层级结构的变化。", "树形视图");
    auto* mainLayout = createPageLayout();
    auto* section = addExampleSection(mainLayout, "项目资源 · ElaTreeView", "点击箭头展开节点，支持多级资源与滚动浏览。");
    auto* resourceModel = new T_TreeViewModel(this);
    _treeView = new ElaTreeView(this);
    _treeView->setModel(resourceModel);
    _treeView->setFixedHeight(380);
    _treeView->setUniformRowHeights(true);
    _treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
    _treeView->expandToDepth(0);
    auto* expandButton = new ElaPushButton("展开全部", this);
    auto* collapseButton = new ElaPushButton("收起全部", this);
    connect(expandButton, &ElaPushButton::clicked, _treeView, &ElaTreeView::expandAll);
    connect(collapseButton, &ElaPushButton::clicked, _treeView, &ElaTreeView::collapseAll);
    auto* actionLayout = new ElaFlowLayout(0, 12, 12);
    actionLayout->addWidget(expandButton);
    actionLayout->addWidget(collapseButton);
    auto* optionsLayout = new ElaFlowLayout(0, 24, 16);
    auto* heightSpinBox = new ElaSpinBox(this);
    heightSpinBox->setRange(24, 60);
    heightSpinBox->setValue(_treeView->getItemHeight());
    heightSpinBox->setFixedWidth(120);
    auto* marginSpinBox = new ElaSpinBox(this);
    marginSpinBox->setRange(0, 20);
    marginSpinBox->setValue(5);
    marginSpinBox->setFixedWidth(120);
    auto* indentSpinBox = new ElaSpinBox(this);
    indentSpinBox->setRange(12, 64);
    indentSpinBox->setValue(20);
    indentSpinBox->setFixedWidth(120);
    connect(heightSpinBox, QOverload<int>::of(&ElaSpinBox::valueChanged), _treeView, &ElaTreeView::setItemHeight);
    connect(marginSpinBox, QOverload<int>::of(&ElaSpinBox::valueChanged), _treeView, &ElaTreeView::setHeaderMargin);
    connect(indentSpinBox, QOverload<int>::of(&ElaSpinBox::valueChanged), _treeView, &ElaTreeView::setIndentation);
    optionsLayout->addWidget(createLabeledControl("行高（像素）", heightSpinBox));
    optionsLayout->addWidget(createLabeledControl("表头间距（像素）", marginSpinBox));
    optionsLayout->addWidget(createLabeledControl("层级缩进（像素）", indentSpinBox));
    section->addLayout(actionLayout);
    section->addWidget(_treeView);
    section->addLayout(optionsLayout);
    section->addWidget(new ElaText(QString("四级资源结构 · 共 %1 个节点").arg(resourceModel->getItemCount()), this));
}

T_TreeView::~T_TreeView()
{
}
