#ifndef ELANAVIGATIONMODEL_H
#define ELANAVIGATIONMODEL_H

#include <QAbstractItemModel>
#include <QObject>

#include "ElaDef.h"
class ElaNavigationNode;
class ElaNavigationModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PRIVATE_CREATE(ElaNavigationNode*, SelectedNode)
    Q_PRIVATE_CREATE(ElaNavigationNode*, SelectedExpandedNode)

public:
    explicit ElaNavigationModel(QObject* parent = nullptr);
    ~ElaNavigationModel();
    QModelIndex parent(const QModelIndex& child) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType::IconName awesome);
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType::IconName awesome);
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, ElaIconType::IconName awesome);
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, ElaIconType::IconName awesome);
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, int keyPoints, ElaIconType::IconName awesome);
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, int keyPoints, ElaIconType::IconName awesome);

    QStringList removeNavigationNode(QString nodeKey);

    ElaNavigationNode* getNavigationNode(QString nodeKey) const;
    QList<ElaNavigationNode*> getRootExpanderNodes() const;
    QList<ElaNavigationNode*> getRootExpandedNodes() const;

private:
    QMap<QString, ElaNavigationNode*> _nodesMap;
    ElaNavigationNode* _rootNode{nullptr};
};

#endif // ELANAVIGATIONMODEL_H
