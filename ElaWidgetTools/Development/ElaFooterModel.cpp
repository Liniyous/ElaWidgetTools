#include "ElaFooterModel.h"

#include "ElaNavigationNode.h"

#include <QUuid>

ElaFooterModel::ElaFooterModel(QObject* parent)
    : QAbstractListModel{parent}
{
    _pSelectedNode = nullptr;
}

ElaFooterModel::~ElaFooterModel()
{
    qDeleteAll(_footerNodeList);
}

int ElaFooterModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : _footerNodeList.count();
}

QVariant ElaFooterModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() && index.row() >= 0 && role == Qt::UserRole)
    {
        if (index.row() < _footerNodeList.count())
        {
            return QVariant::fromValue(_footerNodeList[index.row()]);
        }
    }
    return QVariant();
}

ElaNavigationType::NodeResult ElaFooterModel::addFooterNode(const QString& footerTitle, QString& footerKey, bool isHasFooterPage, int keyPoints, ElaIconType::IconName awesome)
{
    if (_footerNodeList.count() >= 3)
    {
        return ElaNavigationType::FooterUpperLimit;
    }
    ElaNavigationNode* node = new ElaNavigationNode(footerTitle);
    node->setKeyPoints(keyPoints);
    node->setIsFooterNode(true);
    node->setIsHasFooterPage(isHasFooterPage);
    node->setAwesome(awesome);
    footerKey = node->getNodeKey();
    const int row = _footerNodeList.count();
    beginInsertRows(QModelIndex(), row, row);
    _footerNodeList.append(node);
    endInsertRows();
    node->setModelIndex(this->index(_footerNodeList.count() - 1));
    return ElaNavigationType::Success;
}

int ElaFooterModel::getFooterNodeCount() const
{
    return _footerNodeList.count();
}

ElaNavigationNode* ElaFooterModel::getNavigationNode(const QString& footerKey)
{
    for (auto node: _footerNodeList)
    {
        if (node->getNodeKey() == footerKey)
        {
            return node;
        }
    }
    return nullptr;
}

void ElaFooterModel::removeNavigationNode(const QString& footerKey)
{
    for (int row = 0; row < _footerNodeList.count(); row++)
    {
        ElaNavigationNode* node = _footerNodeList[row];
        if (node->getNodeKey() != footerKey)
        {
            continue;
        }
        beginRemoveRows(QModelIndex(), row, row);
        _footerNodeList.removeAt(row);
        if (_pSelectedNode == node)
        {
            _pSelectedNode = nullptr;
        }
        endRemoveRows();
        for (int i = row; i < _footerNodeList.count(); i++)
        {
            _footerNodeList[i]->setModelIndex(index(i, 0));
        }
        node->deleteLater();
        break;
    }
}
