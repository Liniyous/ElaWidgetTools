#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAACTIONCOMMANDERNODE_H
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAACTIONCOMMANDERNODE_H

#include "ElaWidgetToolsDef.h"
#include "ElaPropertyMacro.h"
#include <QModelIndex>
#include <QObject>

class ElaActionCommanderNode : public QObject
{
    Q_OBJECT
    ELA_VARIABLE(QString, NodeName)
    ELA_VARIABLE(int, NodeIndex)
    ELA_VARIABLE(QString, NodeIndexText)
    ELA_VARIABLE(QString, NodeCreateTime)
    ELA_VARIABLE(QList<ElaActionCommanderNode*>, ChildNodes)
    ELA_VARIABLE(ElaActionCommanderNode*, ParentNode)
    ELA_VARIABLE(QModelIndex, ModelIndex)
    ELA_VARIABLE(bool, IsRootNode)
    ELA_VARIABLE(bool, IsCommandNode)
    ELA_VARIABLE(bool, IsCurrentNode)
    ELA_VARIABLE(ElaMessageBarType::MessageMode, MessageMode)
public:
    explicit ElaActionCommanderNode(ElaActionCommanderNode* parent = nullptr);
    ~ElaActionCommanderNode() override;

    void appendChildNode(ElaActionCommanderNode* childNode);
    void removeChildNode(ElaActionCommanderNode* childNode);

    int getRow() const;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAACTIONCOMMANDERNODE_H
