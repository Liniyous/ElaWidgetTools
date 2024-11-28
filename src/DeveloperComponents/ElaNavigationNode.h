#ifndef ELANAVIGATIONNODE_H
#define ELANAVIGATIONNODE_H

#include <QModelIndex>
#include <QObject>

#include "Def.h"

class ElaNavigationNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY_CREATE(QList<ElaNavigationNode*>, ChildrenNodes)
    Q_PRIVATE_CREATE(ElaNavigationNode*, ParentNode)
    Q_PROPERTY_CREATE(ElaIconType::IconName, Awesome)
    Q_PROPERTY_CREATE(QModelIndex, ModelIndex)
    Q_PROPERTY_CREATE(int, KeyPoints)
    Q_PROPERTY_CREATE(int, Depth)
    Q_PROPERTY_CREATE(bool, IsRootNode)
    Q_PROPERTY_CREATE(bool, IsFooterNode)
    Q_PROPERTY_CREATE(bool, IsHasFooterPage)
    Q_PROPERTY_CREATE(bool, IsExpanderNode)
    Q_PROPERTY_CREATE(bool, IsVisible)
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

private:
    QString _nodeKey = "";
    QString _nodeTitle = "";
    bool _isExpanded{false};
};

#endif // ELANAVIGATIONNODE_H
