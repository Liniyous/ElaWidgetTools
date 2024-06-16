#ifndef ELANAVIGATIONNODE_H
#define ELANAVIGATIONNODE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaNavigationNodePrivate;
class ElaNavigationNode : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaNavigationNode)
    Q_PROPERTY_CREATE_Q_H(QList<ElaNavigationNode*>, ChildrenNodes)
    Q_PRIVATE_CREATE_Q_H(ElaNavigationNode*, ParentNode)
    Q_PROPERTY_CREATE_Q_H(ElaIconType, Awesome)
    Q_PROPERTY_CREATE_Q_H(QModelIndex, ModelIndex)
    Q_PROPERTY_CREATE_Q_H(int, KeyPoints)
    Q_PROPERTY_CREATE_Q_H(int, Depth)
    Q_PROPERTY_CREATE_Q_H(bool, IsRootNode)
    Q_PROPERTY_CREATE_Q_H(bool, IsFooterNode)
    Q_PROPERTY_CREATE_Q_H(bool, IsHasFooterPage)
    Q_PROPERTY_CREATE_Q_H(bool, IsExpanderNode)
    Q_PROPERTY_CREATE_Q_H(bool, IsVisible)
public:
    explicit ElaNavigationNode(QString nodeTitle, ElaNavigationNode* parent = nullptr);
    ~ElaNavigationNode();

    QString getNodeKey() const;
    QString getNodeTitle() const;

    void setIsExpanded(bool isExpanded);
    bool getIsExpanded() const;

    void setChildVisible(bool isVisible);
    bool getIsHasChild() const;
    bool getIsHasPageChild() const;

    void appendChildNode(ElaNavigationNode* childNode);

    bool getIsChildHasKeyPoints() const;

    ElaNavigationNode* getOriginalNode();
    bool getIsChildNode(ElaNavigationNode* node);

    int getRow() const;
};

#endif // ELANAVIGATIONNODE_H
