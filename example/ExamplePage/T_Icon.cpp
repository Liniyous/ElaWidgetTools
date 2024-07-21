#include "T_Icon.h"

#include <QStandardItemModel>
#include <QVBoxLayout>

#include "ElaLineEdit.h"
#include "ElaTableView.h"
#include "ElaText.h"
#include "T_IconDelegate.h"
#include "T_IconModel.h"
T_Icon::T_Icon(QWidget* parent)
    : ElaScrollPage(parent)
{
    _metaEnum = QMetaEnum::fromType<ElaIconType>();
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 5, 0);
    centralWidget->setWindowTitle("ElaIcon");
    // TableView
    _iconView = new ElaTableView(this);
    _iconModel = new T_IconModel(this);
    _iconDelegate = new T_IconDelegate(this);
    _iconView->setModel(_iconModel);
    _iconView->setItemDelegate(_iconDelegate);
    _iconView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ElaText* iconTitle = new ElaText("On this page, you can use the search function to use your favorite icons in the program", this);
    iconTitle->setTextPixelSize(15);
    _searchEdit = new ElaLineEdit(this);
    _searchEdit->setPlaceholderText("搜索图标");
    _searchEdit->setFixedSize(300, 35);
    connect(_searchEdit, &ElaLineEdit::textEdited, this, &T_Icon::onSearchEditTextEdit);
    connect(_searchEdit, &ElaLineEdit::focusIn, this, &T_Icon::onSearchEditTextEdit);

    centerVLayout->addWidget(iconTitle);
    centerVLayout->addSpacing(13);
    centerVLayout->addWidget(_searchEdit);
    centerVLayout->addWidget(_iconView);
    this->addCentralWidget(centralWidget);
}

T_Icon::~T_Icon()
{
}

void T_Icon::onSearchEditTextEdit(const QString& searchText)
{
    if (searchText.isEmpty())
    {
        _iconModel->setIsSearchMode(false);
        _iconModel->setSearchKeyList(QStringList());
        _iconView->clearSelection();
        _iconView->viewport()->update();
        return;
    }
    QStringList searchKeyList;
    for (int i = 1; i < _metaEnum.keyCount(); i++)
    {
        QString key = QString(_metaEnum.key(i));
        if (key.contains(searchText, Qt::CaseInsensitive))
        {
            searchKeyList.append(key);
        }
    }
    _iconModel->setSearchKeyList(searchKeyList);
    _iconModel->setIsSearchMode(true);
    _iconView->clearSelection();
    _iconView->scrollTo(_iconModel->index(0, 0));
    _iconView->viewport()->update();
}

void T_Icon::resizeEvent(QResizeEvent* event)
{
    this->_iconModel->setColumnCount(width() / 100);
    ElaScrollPage::resizeEvent(event);
}
