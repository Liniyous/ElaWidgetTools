#include "ElaNavigationNode.h"

#include <QUuid>

ElaNavigationNode::ElaNavigationNode(QString nodeTitle, ElaNavigationNode* parent)
    : QObject(parent)
{
    _pDepth = 0;
    _pKeyPoints = 0;
    _nodeKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    _pNodeTitle = nodeTitle;
    _pIsRootNode = false;
    _pIsFooterNode = false;
    _pIsHasFooterPage = false;
    _pParentNode = parent;
    _pIsExpanderNode = false;
    _pIsVisible = false;
}

ElaNavigationNode::~ElaNavigationNode()
{
    qDeleteAll(_pChildrenNodes);
}

QString ElaNavigationNode::getNodeKey() const
{
    return _nodeKey;
}

void ElaNavigationNode::setIsExpanded(bool isExpanded)
{
    _isExpanded = isExpanded;
    setChildVisible(isExpanded);
}

bool ElaNavigationNode::getIsExpanded() const
{
    return _isExpanded;
}

void ElaNavigationNode::setChildVisible(bool isVisible)
{
    if (isVisible)
    {
        for (auto node: _pChildrenNodes)
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
        for (auto node: _pChildrenNodes)
        {
            node->setChildVisible(isVisible);
            node->setIsVisible(isVisible);
        }
    }
}

bool ElaNavigationNode::getIsHasChild() const
{
    if (_pChildrenNodes.count() > 0)
    {
        return true;
    }
    return false;
}

bool ElaNavigationNode::getIsHasPageChild() const
{
    if (_pChildrenNodes.count() == 0)
    {
        return false;
    }
    for (auto childNode: _pChildrenNodes)
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
    if (_pIsExpanderNode)
    {
        _pChildrenNodes.append(childNode);
    }
}

void ElaNavigationNode::removeChildNode(ElaNavigationNode* childNode)
{
    if (_pIsExpanderNode)
    {
        _pChildrenNodes.removeOne(childNode);
    }
}

bool ElaNavigationNode::getIsChildHasKeyPoints() const
{
    for (auto childNnode: _pChildrenNodes)
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
    if (_pChildrenNodes.count() > 0)
    {
        if (_pChildrenNodes.contains(node))
        {
            return true;
        }
        for (auto childNnode: _pChildrenNodes)
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
    if (_pParentNode)
    {
        return _pParentNode->getChildrenNodes().indexOf(const_cast<ElaNavigationNode*>(this));
    }
    return 0;
}
