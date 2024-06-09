#include "ElaNavigationNode.h"

#include <QUuid>

#include "private/ElaNavigationNodePrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationNode, QList<ElaNavigationNode*>, ChildrenNodes)
Q_PRIVATE_CREATE_Q_CPP(ElaNavigationNode, ElaNavigationNode*, ParentNode)
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationNode, ElaIconType, Awesome)
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationNode, QModelIndex, ModelIndex)
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationNode, int, KeyPoints)
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationNode, int, Depth)
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationNode, bool, IsRootNode)
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationNode, bool, IsFooterNode)
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationNode, bool, IsHasFooterPage)
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationNode, bool, IsExpanderNode)
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationNode, bool, IsVisible)
ElaNavigationNode::ElaNavigationNode(QString nodeTitle, ElaNavigationNode* parent)
    : d_ptr(new ElaNavigationNodePrivate())
{
    Q_D(ElaNavigationNode);
    d->q_ptr = this;
    d->_pDepth = 0;
    d->_pKeyPoints = 0;
    d->_nodeKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    d->_nodeTitle = nodeTitle;
    d->_pIsRootNode = false;
    d->_pIsFooterNode = false;
    d->_pIsHasFooterPage = false;
    d->_pParentNode = parent;
    d->_pIsExpanderNode = false;
    d->_pIsVisible = false;
}

ElaNavigationNode::~ElaNavigationNode()
{
    Q_D(ElaNavigationNode);
    qDeleteAll(d->_pChildrenNodes);
}

QString ElaNavigationNode::getNodeKey() const
{
    return d_ptr->_nodeKey;
}

QString ElaNavigationNode::getNodeTitle() const
{
    return d_ptr->_nodeTitle;
}

void ElaNavigationNode::setIsExpanded(bool isExpanded)
{
    Q_D(ElaNavigationNode);
    d->_isExpanded = isExpanded;
    setChildVisible(isExpanded);
}

bool ElaNavigationNode::getIsExpanded() const
{
    return d_ptr->_isExpanded;
}

void ElaNavigationNode::setChildVisible(bool isVisible)
{
    Q_D(ElaNavigationNode);
    if (isVisible)
    {
        for (auto node : d->_pChildrenNodes)
        {
            node->setIsVisible(isVisible);
            if (node->getIsExpanderNode() && !node->getIsExpanded())
            {
                continue;
            }
            node->setChildVisible(isVisible);
        }
    }
    else
    {
        for (auto node : d->_pChildrenNodes)
        {
            node->setChildVisible(isVisible);
            node->setIsVisible(isVisible);
        }
    }
}

bool ElaNavigationNode::getIsHasChild() const
{
    if (d_ptr->_pChildrenNodes.count() > 0)
    {
        return true;
    }
    return false;
}

bool ElaNavigationNode::getIsHasPageChild() const
{
    if (d_ptr->_pChildrenNodes.count() == 0)
    {
        return false;
    }
    for (auto childNode : d_ptr->_pChildrenNodes)
    {
        if (!childNode->getIsExpanderNode())
        {
            return true;
        }
        if (childNode->getIsHasPageChild())
        {
            return true;
        }
    }
    return false;
}

void ElaNavigationNode::appendChildNode(ElaNavigationNode* childNode)
{
    Q_D(ElaNavigationNode);
    if (d->_pIsExpanderNode)
    {
        d->_pChildrenNodes.append(childNode);
    }
}

bool ElaNavigationNode::getIsChildHasKeyPoints() const
{
    for (auto childNnode : d_ptr->_pChildrenNodes)
    {
        if (childNnode->getKeyPoints())
        {
            return true;
        }
        if (childNnode->getIsChildHasKeyPoints())
        {
            return true;
        }
    }
    return false;
}

ElaNavigationNode* ElaNavigationNode::getOriginalNode()
{
    if (this->getParentNode()->getIsRootNode())
    {
        return this;
    }
    else
    {
        ElaNavigationNode* originalNode = this->getParentNode();
        while (!originalNode->getIsRootNode() && !originalNode->getParentNode()->getIsRootNode())
        {
            originalNode = originalNode->getParentNode();
        }
        return originalNode;
    }
}

bool ElaNavigationNode::getIsChildNode(ElaNavigationNode* node)
{
    if (d_ptr->_pChildrenNodes.count() > 0)
    {
        if (d_ptr->_pChildrenNodes.contains(node))
        {
            return true;
        }
        for (auto childNnode : d_ptr->_pChildrenNodes)
        {
            if (childNnode->getIsChildNode(node))
            {
                return true;
            }
        }
    }
    return false;
}

int ElaNavigationNode::getRow() const
{
    if (d_ptr->_pParentNode)
    {
        return d_ptr->_pParentNode->getChildrenNodes().indexOf(const_cast<ElaNavigationNode*>(this));
    }
    return 0;
}
