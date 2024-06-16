#include "ElaCompactModel.h"

#include "ElaNavigationNode.h"

ElaCompactModel::ElaCompactModel(QObject* parent)
    : QAbstractListModel{parent}
{
    _pSelectedNode = nullptr;
}

ElaCompactModel::~ElaCompactModel()
{
}

ElaNavigationNode* ElaCompactModel::getNavigationNode(QString nodeKey) const
{
    for (auto node : _compactNodeList)
    {
        if (node->getNodeKey() == nodeKey)
        {
            return node;
        }
    }
    return nullptr;
}

QModelIndex ElaCompactModel::getNavigationModelIndex(ElaNavigationNode* node) const
{
    if (node)
    {
        return this->index(_compactNodeList.indexOf(node));
    }
    return QModelIndex();
}

void ElaCompactModel::addCompactNode(ElaNavigationNode* node)
{
    if (!node)
    {
        return;
    }
    beginResetModel();
    _compactNodeList.append(node);
    endResetModel();
}

int ElaCompactModel::rowCount(const QModelIndex& parent) const
{
    return _compactNodeList.count();
}

QVariant ElaCompactModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole)
    {
        if (index.row() < _compactNodeList.count())
        {
            return QVariant::fromValue(_compactNodeList[index.row()]);
        }
    }
    return QVariant();
}
