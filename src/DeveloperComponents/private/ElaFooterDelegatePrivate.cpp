#include "ElaFooterDelegatePrivate.h"

#include "ElaFooterDelegate.h"
#include "ElaNavigationNode.h"
ElaFooterDelegatePrivate::ElaFooterDelegatePrivate(QObject* parent)
    : QObject{parent}
{
}

ElaFooterDelegatePrivate::~ElaFooterDelegatePrivate()
{
}

bool ElaFooterDelegatePrivate::_compareItemY(ElaNavigationNode* node1, ElaNavigationNode* node2)
{
    if (!node1)
    {
        return true;
    }
    if (!node2)
    {
        return false;
    }
    if (node1->getModelIndex().row() < node2->getModelIndex().row())
    {
        return true;
    }
    else
    {
        return false;
    }
}
