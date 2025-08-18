#ifndef ELAFOOTERMODEL_H
#define ELAFOOTERMODEL_H

#include <QAbstractListModel>

#include "ElaDef.h"
class ElaNavigationNode;
class ElaFooterModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PRIVATE_CREATE(ElaNavigationNode*, SelectedNode)
public:
    explicit ElaFooterModel(QObject* parent = nullptr);
    ~ElaFooterModel();
    ElaNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QString& footerKey, bool isHasFooterPage, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None);
    int getFooterNodeCount() const;
    ElaNavigationNode* getNavigationNode(QString footerKey);
    void removeNavigationNode(QString footerKey);

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QList<ElaNavigationNode*> _footerNodeList;
};

#endif // ELAFOOTERMODEL_H
