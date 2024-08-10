#ifndef ELANAVIGATIONMODEL_H
#define ELANAVIGATIONMODEL_H

#include <QAbstractItemModel>
#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaNavigationNode;
class ElaNavigationModelPrivate;
class ElaNavigationModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_Q_CREATE(ElaNavigationModel)
    Q_PRIVATE_CREATE_Q_H(ElaNavigationNode*, SelectedNode)
    Q_PRIVATE_CREATE_Q_H(ElaNavigationNode*, SelectedExpandedNode)

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
    ElaNavigationNode* getNavigationNode(QString nodeKey) const;
};

#endif // ELANAVIGATIONMODEL_H
