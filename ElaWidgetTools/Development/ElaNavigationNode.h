#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELANAVIGATIONNODE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELANAVIGATIONNODE_H_

#include "ElaPropertyMacro.h"
#include <QModelIndex>
#include <QObject>

#include "ElaWidgetToolsDef.h"

class ElaNavigationNode : public QObject
{
    Q_OBJECT
    ELA_VARIABLE(QList<ElaNavigationNode*>, ChildrenNodes)
    ELA_VARIABLE(ElaNavigationNode*, ParentNode)
    ELA_VARIABLE(ElaIconType::IconName, Awesome)
    ELA_VARIABLE(QModelIndex, ModelIndex)
    ELA_VARIABLE(int, KeyPoints)
    ELA_VARIABLE(int, Depth)
    ELA_VARIABLE(bool, IsRootNode)
    ELA_VARIABLE(bool, IsFooterNode)
    ELA_VARIABLE(bool, IsHasFooterPage)
    ELA_VARIABLE(bool, IsExpanderNode)
    ELA_VARIABLE(bool, IsCategoryNode)
    ELA_VARIABLE(bool, IsVisible)
    ELA_VARIABLE(QString, NodeTitle)
public:
    explicit ElaNavigationNode(const QString& nodeTitle, ElaNavigationNode* parent = nullptr);
    ~ElaNavigationNode() override;

    QString getNodeKey() const;

    void setIsExpanded(bool isExpanded);
    bool getIsExpanded() const;

    void setChildVisible(bool isVisible);
    bool getIsHasChild() const;
    bool getIsHasPageChild() const;

    void appendChildNode(ElaNavigationNode* childNode);
    void removeChildNode(ElaNavigationNode* childNode);

    bool getIsChildHasKeyPoints() const;

    ElaNavigationNode* getOriginalNode();
    bool getIsChildNode(ElaNavigationNode* node);

    int getRow() const;
    int getRowExceptCategoryNodes() const;

    QList<ElaNavigationNode*> getExceptCategoryNodes();

private:
    QString _nodeKey = "";
    bool _isExpanded{false};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELANAVIGATIONNODE_H_

