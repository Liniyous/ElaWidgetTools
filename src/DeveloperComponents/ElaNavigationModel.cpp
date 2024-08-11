#include "ElaNavigationModel.h"

#include <QIcon>
#include <QJsonObject>

#include "ElaNavigationNode.h"
ElaNavigationModel::ElaNavigationModel(QObject* parent)
    : QAbstractItemModel{parent}
{
    _rootNode = new ElaNavigationNode("root");
    _rootNode->setIsRootNode(true);
    _rootNode->setIsExpanderNode(true);
    _pSelectedNode = nullptr;
    _pSelectedExpandedNode = nullptr;
}

ElaNavigationModel::~ElaNavigationModel()
{
    delete _rootNode;
}

QModelIndex ElaNavigationModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
    {
        return QModelIndex();
    }
    ElaNavigationNode* childNode = static_cast<ElaNavigationNode*>(child.internalPointer());
    ElaNavigationNode* parentNode = childNode->getParentNode();
    if (parentNode == _rootNode)
    {
        return QModelIndex();
    }
    else if (parentNode == nullptr)
    {
        return QModelIndex();
    }
    return createIndex(parentNode->getRow(), 0, parentNode);
}

QModelIndex ElaNavigationModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }
    ElaNavigationNode* parentNode;
    if (!parent.isValid())
    {
        parentNode = _rootNode;
    }
    else
    {
        parentNode = static_cast<ElaNavigationNode*>(parent.internalPointer());
    }
    ElaNavigationNode* childNode = nullptr;
    if (parentNode->getChildrenNodes().count() > row)
    {
        childNode = parentNode->getChildrenNodes().at(row);
    }
    if (childNode)
    {
        return createIndex(row, column, childNode);
    }
    return QModelIndex();
}

int ElaNavigationModel::rowCount(const QModelIndex& parent) const
{
    ElaNavigationNode* parentNode;
    if (parent.column() > 0)
    {
        return 0;
    }
    if (!parent.isValid())
    {
        parentNode = _rootNode;
    }
    else
    {
        parentNode = static_cast<ElaNavigationNode*>(parent.internalPointer());
    }
    return parentNode->getChildrenNodes().count();
};

int ElaNavigationModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
};

QVariant ElaNavigationModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)
    return QVariant();
}

ElaNavigationType::NodeOperateReturnType ElaNavigationModel::addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType::IconName awesome)
{
    ElaNavigationNode* node = new ElaNavigationNode(expanderTitle, _rootNode);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setIsExpanderNode(true);
    node->setAwesome(awesome);
    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    expanderKey = node->getNodeKey();
    return ElaNavigationType::NodeOperateReturnType::Success;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationModel::addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType::IconName awesome)
{
    if (!_nodesMap.contains(targetExpanderKey))
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeInvalid;
    }
    ElaNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode())
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeTypeError;
    }
    if (parentNode->getDepth() > 10)
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeDepthLimit;
    }
    ElaNavigationNode* node = new ElaNavigationNode(expanderTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setIsExpanderNode(true);
    node->setAwesome(awesome);
    if (parentNode->getIsVisible() && parentNode->getIsExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    expanderKey = node->getNodeKey();
    return ElaNavigationType::NodeOperateReturnType::Success;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationModel::addPageNode(QString pageTitle, QString& pageKey, ElaIconType::IconName awesome)
{
    ElaNavigationNode* node = new ElaNavigationNode(pageTitle, _rootNode);
    node->setAwesome(awesome);
    node->setDepth(1);
    node->setIsVisible(true);
    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    pageKey = node->getNodeKey();
    if (!_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    return ElaNavigationType::NodeOperateReturnType::Success;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationModel::addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, ElaIconType::IconName awesome)
{
    if (!_nodesMap.contains(targetExpanderKey))
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeInvalid;
    }
    ElaNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode())
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeTypeError;
    }
    if (parentNode->getDepth() > 10)
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeDepthLimit;
    }
    ElaNavigationNode* node = new ElaNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setAwesome(awesome);
    if (parentNode->getIsVisible() && parentNode->getIsExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    pageKey = node->getNodeKey();
    if (!_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    return ElaNavigationType::NodeOperateReturnType::Success;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationModel::addPageNode(QString pageTitle, QString& pageKey, int keyPoints, ElaIconType::IconName awesome)
{
    ElaNavigationNode* node = new ElaNavigationNode(pageTitle, _rootNode);
    node->setAwesome(awesome);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setKeyPoints(keyPoints);
    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    pageKey = node->getNodeKey();
    if (!_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    return ElaNavigationType::NodeOperateReturnType::Success;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationModel::addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, int keyPoints, ElaIconType::IconName awesome)
{
    if (!_nodesMap.contains(targetExpanderKey))
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeInvalid;
    }
    ElaNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode())
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeTypeError;
    }
    if (parentNode->getDepth() > 10)
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeDepthLimit;
    }
    ElaNavigationNode* node = new ElaNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setAwesome(awesome);
    node->setKeyPoints(keyPoints);
    if (parentNode->getIsVisible() && parentNode->getIsExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    pageKey = node->getNodeKey();
    if (!_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    return ElaNavigationType::NodeOperateReturnType::Success;
}

ElaNavigationNode* ElaNavigationModel::getNavigationNode(QString nodeKey) const
{
    if (_nodesMap.contains(nodeKey))
    {
        return _nodesMap.value(nodeKey);
    }
    return nullptr;
}
