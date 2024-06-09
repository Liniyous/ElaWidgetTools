#ifndef ELANAVIGATIONNODEPRIVATE_H
#define ELANAVIGATIONNODEPRIVATE_H

#include <QModelIndex>
#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaNavigationNode;
class ElaNavigationNodePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaNavigationNode)
    Q_PROPERTY_CREATE_D(QList<ElaNavigationNode*>, ChildrenNodes)
    Q_PRIVATE_CREATE_D(ElaNavigationNode*, ParentNode)
    Q_PRIVATE_CREATE_D(ElaIconType, Awesome)
    Q_PRIVATE_CREATE_D(QModelIndex, ModelIndex)
    Q_PROPERTY_CREATE_D(int, KeyPoints)
    Q_PROPERTY_CREATE_D(int, Depth)
    Q_PROPERTY_CREATE_D(bool, IsRootNode)
    Q_PROPERTY_CREATE_D(bool, IsFooterNode)
    Q_PROPERTY_CREATE_D(bool, IsHasFooterPage)
    Q_PROPERTY_CREATE_D(bool, IsExpanderNode)
    Q_PROPERTY_CREATE_D(bool, IsVisible)
public:
    explicit ElaNavigationNodePrivate(QObject* parent = nullptr);
    ~ElaNavigationNodePrivate();

private:
    QString _nodeKey = "";
    QString _nodeTitle = "";
    bool _isExpanded{false};
};

#endif // ELANAVIGATIONNODEPRIVATE_H
