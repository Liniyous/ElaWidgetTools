#include "ElaNavigationSuggestModel.h"

ElaNavigationSuggestModel::ElaNavigationSuggestModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

ElaNavigationSuggestModel::~ElaNavigationSuggestModel()
{
}

int ElaNavigationSuggestModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return _nodesVector.count();
}

QVariant ElaNavigationSuggestModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void ElaNavigationSuggestModel::setSearchNode(QVector<ElaNavigationNode*> node)
{
    if (node.count() == 0)
    {
        return;
    }
    beginResetModel();
    this->_nodesVector = node;
    endResetModel();
}

void ElaNavigationSuggestModel::clearSearchNode()
{
    this->_nodesVector.clear();
}

ElaNavigationNode* ElaNavigationSuggestModel::getSearchNode(int row)
{
    if (row >= _nodesVector.count())
    {
        return nullptr;
    }
    return _nodesVector[row];
}
