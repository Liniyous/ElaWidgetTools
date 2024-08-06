#include "ElaFooterModel.h"

#include <QUuid>

#include "ElaNavigationNode.h"
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
    return _footerNodeList.count();
}

QVariant ElaFooterModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole)
    {
        if (index.row() < _footerNodeList.count())
        {
            return QVariant::fromValue(_footerNodeList[index.row()]);
        }
    }
    return QVariant();
}

ElaNavigationType::NodeOperateReturnType ElaFooterModel::addFooterNode(QString footerTitle, QString& footerKey, bool isHasFooterPage, int keyPoints, ElaIconType::IconName awesome)
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
    beginResetModel();
    _footerNodeList.append(node);
    endResetModel();
    node->setModelIndex(this->index(_footerNodeList.count() - 1));
    return ElaNavigationType::Success;
}

int ElaFooterModel::getFooterNodeCount() const
{
    return _footerNodeList.count();
}

ElaNavigationNode* ElaFooterModel::getNavigationNode(QString footerKey)
{
    for (auto node : _footerNodeList)
    {
        if (node->getNodeKey() == footerKey)
        {
            return node;
        }
    }
    return nullptr;
}
