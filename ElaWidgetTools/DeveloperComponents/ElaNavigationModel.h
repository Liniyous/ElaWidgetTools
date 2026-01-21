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
    ~ElaNavigationModel() override;

    void setIsMaximalMode(bool isMaximal);
    bool getIsMaximalMode();

    QModelIndex parent(const QModelIndex& child) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    ElaNavigationType::NodeResult addExpanderNode(const QString& expanderTitle, QString& expanderKey, ElaIconType::IconName awesome);
    ElaNavigationType::NodeResult addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, ElaIconType::IconName awesome);
    ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QString& pageKey, ElaIconType::IconName awesome);
    ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, ElaIconType::IconName awesome);
    ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QString& pageKey, int keyPoints, ElaIconType::IconName awesome);
    ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, int keyPoints, ElaIconType::IconName awesome);
    ElaNavigationType::NodeResult addCategoryNode(const QString& categoryTitle, QString& categoryKey);
    ElaNavigationType::NodeResult addCategoryNode(const QString& categoryTitle, QString& categoryKey, const QString& targetExpanderKey);

    QStringList removeNavigationNode(const QString& nodeKey);

    ElaNavigationNode* getNavigationNode(const QString& nodeKey) const;
    QList<ElaNavigationNode*> getRootExpanderNodes() const;
    QList<ElaNavigationNode*> getRootExpandedNodes() const;

    QList<ElaNavigationNode*> getRootCategoryNodes() const;

private:
    bool _isMaximalMode{true};
    QMap<QString, ElaNavigationNode*> _nodesMap;
    ElaNavigationNode* _rootNode{nullptr};
};

#endif // ELANAVIGATIONMODEL_H
