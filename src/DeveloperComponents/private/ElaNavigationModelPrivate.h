#ifndef ELANAVIGATIONMODELPRIVATE_H
#define ELANAVIGATIONMODELPRIVATE_H

#include <QMap>
#include <QModelIndex>
#include <QObject>

#include "stdafx.h"
class ElaNavigationNode;
class ElaNavigationModel;
class ElaNavigationModelPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaNavigationModel)
    Q_PRIVATE_CREATE_D(ElaNavigationNode*, SelectedNode)
    Q_PRIVATE_CREATE_D(ElaNavigationNode*, SelectedExpandedNode)
public:
    explicit ElaNavigationModelPrivate(QObject* parent = nullptr);
    ~ElaNavigationModelPrivate();

private:
    mutable QMap<QString, ElaNavigationNode*> _nodesMap;
    ElaNavigationNode* _rootNode{nullptr};
};

#endif // ELANAVIGATIONMODELPRIVATE_H
