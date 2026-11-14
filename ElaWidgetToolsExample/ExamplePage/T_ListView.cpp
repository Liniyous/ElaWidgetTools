#include "T_ListView.h"

#include "ElaCheckBox.h"
#include "ElaFlowLayout.h"
#include "ElaLineEdit.h"
#include "ElaListView.h"
#include "ElaText.h"
#include "T_ListViewModel.h"

#include <QItemSelectionModel>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>

T_ListView::T_ListView(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("列表视图");
    createCustomWidget("搜索列表内容，尝试单选、多选以及不同的行背景。", "列表视图");
    auto* mainLayout = createPageLayout();
    auto* section = addExampleSection(mainLayout, "资源列表 · ElaListView", "启用多选后，可使用 Ctrl 或 Shift 连续选择多个条目。");
    auto* searchEdit = new ElaLineEdit(this);
    searchEdit->setFixedWidth(260);
    searchEdit->setPlaceholderText("搜索资源名称");
    searchEdit->setClearButtonEnabled(true);
    auto* multiCheckBox = new ElaCheckBox("允许多选", this);
    multiCheckBox->setFixedHeight(searchEdit->height());
    auto* alternatingCheckBox = new ElaCheckBox("交替行背景", this);
    alternatingCheckBox->setFixedHeight(searchEdit->height());
    auto* optionsLayout = new ElaFlowLayout(0, 20, 12);
    optionsLayout->addWidget(searchEdit);
    optionsLayout->addWidget(multiCheckBox);
    optionsLayout->addWidget(alternatingCheckBox);
    _listView = new ElaListView(this);
    _listView->setFixedHeight(350);
    _listView->setSelectionMode(QAbstractItemView::SingleSelection);
    auto* resourceModel = new T_ListViewModel(this);
    auto* filterModel = new QSortFilterProxyModel(this);
    filterModel->setSourceModel(resourceModel);
    filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _listView->setModel(filterModel);
    auto* resultText = new ElaText("选择结果：尚未选择", this);
    connect(searchEdit, &ElaLineEdit::textChanged, filterModel, &QSortFilterProxyModel::setFilterFixedString);
    connect(multiCheckBox, &ElaCheckBox::toggled, this, [this](bool checked) {
        _listView->clearSelection();
        _listView->setSelectionMode(checked ? QAbstractItemView::ExtendedSelection : QAbstractItemView::SingleSelection);
    });
    connect(alternatingCheckBox, &ElaCheckBox::toggled, _listView, &ElaListView::setAlternatingRowColors);
    connect(_listView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this, resultText]() {
        const auto indexes = _listView->selectionModel()->selectedIndexes();
        resultText->setText(indexes.isEmpty() ? "选择结果：尚未选择" : QString("已选 %1 项 · %2").arg(indexes.size()).arg(indexes.first().data().toString()));
    });
    resultText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    section->addLayout(optionsLayout);
    section->addWidget(_listView);
    section->addWidget(resultText);
}

T_ListView::~T_ListView()
{
}
