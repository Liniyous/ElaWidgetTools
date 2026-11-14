#include "T_TableView.h"

#include "ElaCheckBox.h"
#include "ElaFlowLayout.h"
#include "ElaLineEdit.h"
#include "ElaTableView.h"
#include "ElaText.h"
#include "T_TableViewModel.h"

#include <QHeaderView>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>

T_TableView::T_TableView(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("表格视图");
    createCustomWidget("在多列数据中搜索、排序与选择，拖动表头边界可调整列宽。", "表格视图");
    auto* mainLayout = createPageLayout();
    auto* section = addExampleSection(mainLayout, "音乐资料库 · ElaTableView", "输入歌名、歌手或专辑筛选，点击列标题切换排序。");
    auto* searchEdit = new ElaLineEdit(this);
    searchEdit->setFixedWidth(280);
    searchEdit->setPlaceholderText("搜索歌名、歌手或专辑");
    searchEdit->setClearButtonEnabled(true);
    auto* alternatingCheckBox = new ElaCheckBox("交替行背景", this);
    alternatingCheckBox->setFixedHeight(searchEdit->height());
    alternatingCheckBox->setChecked(true);
    auto* optionsLayout = new ElaFlowLayout(0, 20, 12);
    optionsLayout->addWidget(searchEdit);
    optionsLayout->addWidget(alternatingCheckBox);
    _tableView = new ElaTableView(this);
    auto* libraryModel = new T_TableViewModel(this);
    auto* filterModel = new T_MusicFilterModel(this);
    filterModel->setSourceModel(libraryModel);
    filterModel->setFilterKeyColumn(-1);
    filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _tableView->setModel(filterModel);
    _tableView->setAlternatingRowColors(true);
    // 初始保持源模型顺序 点击表头后再按对应列排序
    _tableView->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
    _tableView->setSortingEnabled(true);
    _tableView->setIconSize(QSize(32, 32));
    _tableView->verticalHeader()->hide();
    _tableView->verticalHeader()->setDefaultSectionSize(44);
    _tableView->horizontalHeader()->setMinimumSectionSize(48);
    _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _tableView->setColumnWidth(0, 52);
    _tableView->setColumnWidth(2, 130);
    _tableView->setColumnWidth(3, 150);
    _tableView->setColumnWidth(4, 68);
    _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _tableView->setFixedHeight(380);
    auto* resultText = new ElaText("点击一行查看选择结果", this);
    resultText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    connect(searchEdit, &ElaLineEdit::textChanged, filterModel, &QSortFilterProxyModel::setFilterFixedString);
    connect(alternatingCheckBox, &ElaCheckBox::toggled, _tableView, &ElaTableView::setAlternatingRowColors);
    connect(_tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, [resultText](const QModelIndex& current) {
        resultText->setText(current.isValid() ? QString("当前选择：%1 · %2").arg(current.sibling(current.row(), 1).data().toString(), current.sibling(current.row(), 2).data().toString()) : "点击一行查看选择结果");
    });
    section->addLayout(optionsLayout);
    section->addWidget(_tableView);
    section->addWidget(resultText);
}

T_TableView::~T_TableView()
{
}
