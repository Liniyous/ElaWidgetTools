#include "ElaNavigationDelegatePrivate.h"

#include <QModelIndex>

#include "ElaNavigationNode.h"
ElaNavigationDelegatePrivate::ElaNavigationDelegatePrivate(QObject* parent)
    : QObject{parent}
{
}

ElaNavigationDelegatePrivate::~ElaNavigationDelegatePrivate()
{
}

bool ElaNavigationDelegatePrivate::_compareItemY(ElaNavigationNode* node1, ElaNavigationNode* node2)
{
    // 返回TRUE 即node1 高于 node2
    if (!node1)
    {
        return false;
    }
    if (!node2)
    {
        return true;
    }
    // 同一父节点
    if (node1->getParentNode() == node2->getParentNode())
    {
        if (node1->getModelIndex().row() < node2->getModelIndex().row())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        ElaNavigationNode* node1OriginalNode = node1->getOriginalNode();
        ElaNavigationNode* node2OriginalNode = node2->getOriginalNode();
        // 不同父节点  相同起源节点
        if (node1OriginalNode == node2OriginalNode)
        {
            int node1Depth = node1->getDepth();
            int node2Depth = node2->getDepth();
            // 相同深度
            if (node1Depth == node2Depth)
            {
                ElaNavigationNode* node1ParentNode = node1->getParentNode();
                ElaNavigationNode* node2ParentNode = node2->getParentNode();
                if (node1ParentNode->getModelIndex().row() < node2ParentNode->getModelIndex().row())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if (node1Depth < node2Depth)
                {
                    ElaNavigationNode* node2ParentNode = node2->getParentNode();
                    while (node2ParentNode->getDepth() != node1Depth)
                    {
                        node2ParentNode = node2ParentNode->getParentNode();
                    }
                    // 父子节点关系
                    if (node1 == node2ParentNode)
                    {
                        return true;
                    }
                    if (node1->getModelIndex().row() < node2ParentNode->getModelIndex().row())
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    ElaNavigationNode* node1ParentNode = node1->getParentNode();
                    while (node1ParentNode->getDepth() != node2Depth)
                    {
                        node1ParentNode = node1ParentNode->getParentNode();
                    }
                    if (node2 == node1ParentNode)
                    {
                        return false;
                    }
                    if (node1ParentNode->getModelIndex().row() < node2->getModelIndex().row())
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
        else
        {
            if (node1OriginalNode->getModelIndex().row() < node2OriginalNode->getModelIndex().row())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
