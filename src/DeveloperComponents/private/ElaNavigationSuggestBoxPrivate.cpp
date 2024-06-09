#include "ElaNavigationSuggestBoxPrivate.h"

#include "DeveloperComponents/ElaNavigationNode.h"
#include "DeveloperComponents/ElaNavigationSuggestBox.h"
#include "DeveloperComponents/ElaNavigationSuggestModel.h"
#include "DeveloperComponents/ElaNavigationSuggestView.h"
#include "ElaApplication.h"
#include "ElaLineEdit.h"
ElaNavigationSuggestBoxPrivate::ElaNavigationSuggestBoxPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaNavigationSuggestBoxPrivate::~ElaNavigationSuggestBoxPrivate()
{
}

void ElaNavigationSuggestBoxPrivate::onWMWindowClickedEvent(QMap<QString, QVariant> data)
{
    Q_Q(ElaNavigationSuggestBox);
    Q_UNUSED(data)
    if (ElaApplication::containsCursorToItem(_searchView) || ElaApplication::containsCursorToItem(q))
    {
        return;
    }
    _searchModel->clearSearchNode();
    _searchView->hide();
}

void ElaNavigationSuggestBoxPrivate::onSearchEditTextEdit(const QString& searchText)
{
    if (searchText.isEmpty())
    {
        _searchView->hide();
        return;
    }
    QVector<ElaNavigationNode*> nodesVector;
    for (const auto& node : _pageNodeList)
    {
        if (node->getNodeTitle().contains(searchText, Qt::CaseInsensitive))
        {
            nodesVector.append(node);
        }
    }
    if (nodesVector.count())
    {
        _searchModel->setSearchNode(nodesVector);
        int rowCount = nodesVector.count();
        if (rowCount > 4)
        {
            rowCount = 4;
        }
        if (!_searchView->isVisible())
        {
            _searchView->resize(280, 40 * rowCount + 16);
            _searchView->show();
        }
        else
        {
            _searchView->resize(280, 40 * rowCount + 16);
        }
    }
    else
    {
        _searchModel->clearSearchNode();
        _searchView->hide();
    }
}

void ElaNavigationSuggestBoxPrivate::onSearchViewPressed(const QModelIndex& index)
{
    Q_Q(ElaNavigationSuggestBox);
    _searchView->hide();
    _searchEdit->clear();
    _searchView->clearSelection();
    if (!index.isValid())
    {
        return;
    }
    ElaNavigationNode* node = _searchModel->getSearchNode(index.row());
    Q_EMIT q->searchNodeClicked(node);
    _searchModel->clearSearchNode();
}

void ElaNavigationSuggestBoxPrivate::_raiseSearchView()
{
    Q_Q(ElaNavigationSuggestBox);
    _searchView->raise();
    q->raise();
}

void ElaNavigationSuggestBoxPrivate::_appendPageNode(ElaNavigationNode* node)
{
    _pageNodeList.append(node);
}
