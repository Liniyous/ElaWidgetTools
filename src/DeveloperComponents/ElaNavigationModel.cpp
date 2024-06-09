#include "ElaNavigationModel.h"

#include <QIcon>
#include <QJsonObject>

#include "ElaNavigationNode.h"
#include "private/ElaNavigationModelPrivate.h"
Q_PRIVATE_CREATE_Q_CPP(ElaNavigationModel, ElaNavigationNode*, SelectedNode)
Q_PRIVATE_CREATE_Q_CPP(ElaNavigationModel, ElaNavigationNode*, SelectedExpandedNode)
ElaNavigationModel::ElaNavigationModel(QObject* parent)
    : QAbstractItemModel{parent}, d_ptr(new ElaNavigationModelPrivate())
{
    Q_D(ElaNavigationModel);
    d->q_ptr = this;
    d->_rootNode = new ElaNavigationNode("root");
    d->_rootNode->setIsRootNode(true);
    d->_rootNode->setIsExpanderNode(true);
    d->_pSelectedNode = nullptr;
    d->_pSelectedExpandedNode = nullptr;
}

ElaNavigationModel::~ElaNavigationModel()
{
    Q_D(ElaNavigationModel);
    delete d->_rootNode;
}

QModelIndex ElaNavigationModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
    {
        return QModelIndex();
    }
    ElaNavigationNode* childNode = static_cast<ElaNavigationNode*>(child.internalPointer());
    ElaNavigationNode* parentNode = childNode->getParentNode();
    if (parentNode == d_ptr->_rootNode)
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
        parentNode = d_ptr->_rootNode;
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
        parentNode = d_ptr->_rootNode;
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

ElaNavigationType::NodeOperateReturnType ElaNavigationModel::addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType awesome) const
{
    ElaNavigationNode* node = new ElaNavigationNode(expanderTitle, d_ptr->_rootNode);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setIsExpanderNode(true);
    node->setAwesome(awesome);
    d_ptr->_rootNode->appendChildNode(node);
    d_ptr->_nodesMap.insert(node->getNodeKey(), node);
    expanderKey = node->getNodeKey();
    return ElaNavigationType::NodeOperateReturnType::Success;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationModel::addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType awesome) const
{
    if (!d_ptr->_nodesMap.contains(targetExpanderKey))
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeInvalid;
    }
    ElaNavigationNode* parentNode = d_ptr->_nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode())
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeTypeError;
    }
    if (parentNode->getDepth() > 3)
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeDepthLimit;
    }
    ElaNavigationNode* node = new ElaNavigationNode(expanderTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setIsExpanderNode(true);
    node->setAwesome(awesome);
    parentNode->appendChildNode(node);
    d_ptr->_nodesMap.insert(node->getNodeKey(), node);
    expanderKey = node->getNodeKey();
    return ElaNavigationType::NodeOperateReturnType::Success;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationModel::addPageNode(QString pageTitle, QString& pageKey, ElaIconType awesome) const
{
    ElaNavigationNode* node = new ElaNavigationNode(pageTitle, d_ptr->_rootNode);
    node->setAwesome(awesome);
    node->setDepth(1);
    node->setIsVisible(true);
    d_ptr->_rootNode->appendChildNode(node);
    d_ptr->_nodesMap.insert(node->getNodeKey(), node);
    pageKey = node->getNodeKey();
    if (!d_ptr->_pSelectedNode)
    {
        d_ptr->_pSelectedNode = node;
    }
    return ElaNavigationType::NodeOperateReturnType::Success;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationModel::addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, ElaIconType awesome) const
{
    if (!d_ptr->_nodesMap.contains(targetExpanderKey))
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeInvalid;
    }
    ElaNavigationNode* parentNode = d_ptr->_nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode())
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeTypeError;
    }
    if (parentNode->getDepth() > 3)
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeDepthLimit;
    }
    ElaNavigationNode* node = new ElaNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setAwesome(awesome);
    parentNode->appendChildNode(node);
    d_ptr->_nodesMap.insert(node->getNodeKey(), node);
    pageKey = node->getNodeKey();
    if (!d_ptr->_pSelectedNode)
    {
        d_ptr->_pSelectedNode = node;
    }
    return ElaNavigationType::NodeOperateReturnType::Success;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationModel::addPageNode(QString pageTitle, QString& pageKey, int keyPoints, ElaIconType awesome) const
{
    ElaNavigationNode* node = new ElaNavigationNode(pageTitle, d_ptr->_rootNode);
    node->setAwesome(awesome);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setKeyPoints(keyPoints);
    d_ptr->_rootNode->appendChildNode(node);
    d_ptr->_nodesMap.insert(node->getNodeKey(), node);
    pageKey = node->getNodeKey();
    if (!d_ptr->_pSelectedNode)
    {
        d_ptr->_pSelectedNode = node;
    }
    return ElaNavigationType::NodeOperateReturnType::Success;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationModel::addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, int keyPoints, ElaIconType awesome) const
{
    if (!d_ptr->_nodesMap.contains(targetExpanderKey))
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeInvalid;
    }
    ElaNavigationNode* parentNode = d_ptr->_nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode())
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeTypeError;
    }
    if (parentNode->getDepth() > 3)
    {
        return ElaNavigationType::NodeOperateReturnType::TargetNodeDepthLimit;
    }
    ElaNavigationNode* node = new ElaNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setAwesome(awesome);
    node->setKeyPoints(keyPoints);
    parentNode->appendChildNode(node);
    d_ptr->_nodesMap.insert(node->getNodeKey(), node);
    pageKey = node->getNodeKey();
    if (!d_ptr->_pSelectedNode)
    {
        d_ptr->_pSelectedNode = node;
    }
    return ElaNavigationType::NodeOperateReturnType::Success;
}

ElaNavigationNode* ElaNavigationModel::getNavigationNode(QString nodeKey) const
{
    if (d_ptr->_nodesMap.contains(nodeKey))
    {
        return d_ptr->_nodesMap.value(nodeKey);
    }
    return nullptr;
}
