#include "ElaNavigationModel.h"

#include "ElaNavigationNode.h"

#include <QIcon>
#include <QJsonObject>

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
    _rootNode->deleteLater();
}

QModelIndex ElaNavigationModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
    {
        return {};
    }
    ElaNavigationNode* childNode = static_cast<ElaNavigationNode*>(child.internalPointer());
    ElaNavigationNode* parentNode = childNode->getParentNode();
    if (parentNode == _rootNode)
    {
        return {};
    }
    if (parentNode == nullptr)
    {
        return {};
    }
    return createIndex(parentNode->getRow(), 0, parentNode);
}

QModelIndex ElaNavigationModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return {};
    }
    ElaNavigationNode* parentNode = parent.isValid() ? static_cast<ElaNavigationNode*>(parent.internalPointer()) : _rootNode;
    ElaNavigationNode* childNode = parentNode->getChildrenNodes().at(row);
    childNode->setModelIndex(createIndex(row, column, childNode));
    return childNode->getModelIndex();
}

int ElaNavigationModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0)
    {
        return 0;
    }
    ElaNavigationNode* parentNode = parent.isValid() ? static_cast<ElaNavigationNode*>(parent.internalPointer()) : _rootNode;
    return parentNode->getChildrenNodes().count();
}

int ElaNavigationModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant ElaNavigationModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)
    return QVariant();
}

ElaNavigationType::NodeResult ElaNavigationModel::addExpanderNode(const QString& expanderTitle, QString& expanderKey, ElaIconType::IconName awesome)
{
    return _addNode(expanderTitle, expanderKey, _rootNode, NodeType::Expander, 0, awesome);
}

ElaNavigationType::NodeResult ElaNavigationModel::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, ElaIconType::IconName awesome)
{
    return _addNode(expanderTitle, expanderKey, getNavigationNode(targetExpanderKey), NodeType::Expander, 0, awesome);
}

ElaNavigationType::NodeResult ElaNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, ElaIconType::IconName awesome)
{
    return _addNode(pageTitle, pageKey, _rootNode, NodeType::Page, 0, awesome);
}

ElaNavigationType::NodeResult ElaNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, ElaIconType::IconName awesome)
{
    return _addNode(pageTitle, pageKey, getNavigationNode(targetExpanderKey), NodeType::Page, 0, awesome);
}

ElaNavigationType::NodeResult ElaNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, int keyPoints, ElaIconType::IconName awesome)
{
    return _addNode(pageTitle, pageKey, _rootNode, NodeType::Page, keyPoints, awesome);
}

ElaNavigationType::NodeResult ElaNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, int keyPoints, ElaIconType::IconName awesome)
{
    return _addNode(pageTitle, pageKey, getNavigationNode(targetExpanderKey), NodeType::Page, keyPoints, awesome);
}

ElaNavigationType::NodeResult ElaNavigationModel::addCategoryNode(const QString& categoryTitle, QString& categoryKey)
{
    return _addNode(categoryTitle, categoryKey, _rootNode, NodeType::Category, 0, ElaIconType::None);
}

ElaNavigationType::NodeResult ElaNavigationModel::addCategoryNode(const QString& categoryTitle, QString& categoryKey, const QString& targetExpanderKey)
{
    return _addNode(categoryTitle, categoryKey, getNavigationNode(targetExpanderKey), NodeType::Category, 0, ElaIconType::None);
}

QStringList ElaNavigationModel::removeNavigationNode(const QString& nodeKey)
{
    QList<QString> removeKeyList;
    if (!_nodesMap.contains(nodeKey))
    {
        return removeKeyList;
    }
    ElaNavigationNode* node = _nodesMap.value(nodeKey);
    ElaNavigationNode* parentNode = node->getParentNode();
    if (node->getIsExpanderNode())
    {
        QList<ElaNavigationNode*> childNodeList = node->getChildrenNodes();
        for (int i = 0; i < childNodeList.count(); i++)
        {
            ElaNavigationNode* childNode = childNodeList[i];
            QList<QString> childRemoveKeyList = removeNavigationNode(childNode->getNodeKey());
            removeKeyList.append(childRemoveKeyList);
        }
    }
    else
    {
        removeKeyList.append(node->getNodeKey());
    }
    _nodesMap.remove(node->getNodeKey());
    int removeRow = parentNode->getChildrenNodes().indexOf(node);
    beginRemoveRows(parentNode->getModelIndex(), removeRow, removeRow);
    parentNode->removeChildNode(node);
    endRemoveRows();
    node->deleteLater();
    return removeKeyList;
}

ElaNavigationNode* ElaNavigationModel::getNavigationNode(const QString& nodeKey) const
{
    if (_nodesMap.contains(nodeKey))
    {
        return _nodesMap.value(nodeKey);
    }
    return nullptr;
}

QList<ElaNavigationNode*> ElaNavigationModel::getRootExpanderNodes() const
{
    QList<ElaNavigationNode*> expandedNodeList;
    for (auto node: _rootNode->getChildrenNodes())
    {
        if (node->getIsExpanderNode())
        {
            expandedNodeList.append(node);
        }
    }
    return expandedNodeList;
}

QList<ElaNavigationNode*> ElaNavigationModel::getRootExpandedNodes() const
{
    QList<ElaNavigationNode*> expandedNodeList;
    for (auto node: _rootNode->getChildrenNodes())
    {
        if (node->getIsExpanderNode() && node->getIsExpanded())
        {
            expandedNodeList.append(node);
        }
    }
    return expandedNodeList;
}

QList<ElaNavigationNode*> ElaNavigationModel::getRootCategoryNodes() const
{
    QList<ElaNavigationNode*> categoryNodeList;
    for (auto node: _rootNode->getChildrenNodes())
    {
        if (node->getIsCategoryNode())
        {
            categoryNodeList.append(node);
        }
    }
    return categoryNodeList;
}

ElaNavigationType::NodeResult ElaNavigationModel::_addNode(const QString& title, QString& key, ElaNavigationNode* parentNode, NodeType type, int keyPoints, ElaIconType::IconName icon)
{
    if (!parentNode)
    {
        return ElaNavigationType::TargetNodeInvalid;
    }
    if (!parentNode->getIsExpanderNode())
    {
        return ElaNavigationType::TargetNodeTypeError;
    }
    if (parentNode->getDepth() > 10)
    {
        return ElaNavigationType::TargetNodeDepthLimit;
    }
    auto node = new ElaNavigationNode(title, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setAwesome(icon);
    node->setKeyPoints(keyPoints);
    node->setIsExpanderNode(type == NodeType::Expander);
    node->setIsCategoryNode(type == NodeType::Category);
    node->setIsVisible(parentNode->getIsRootNode() || (parentNode->getIsVisible() && parentNode->getIsExpanded()));
    const int row = parentNode->getChildrenNodes().count();
    beginInsertRows(parentNode->getModelIndex(), row, row);
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    // 追加节点只建立新索引，已有节点的行号保持稳定
    node->setModelIndex(createIndex(row, 0, node));
    if (type == NodeType::Page && !_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    endInsertRows();
    key = node->getNodeKey();
    return ElaNavigationType::Success;
}
