#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAFOOTERMODEL_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAFOOTERMODEL_H_

#include "ElaPropertyMacro.h"
#include <QAbstractListModel>

#include "ElaWidgetToolsDef.h"
class ElaNavigationNode;
class ElaFooterModel : public QAbstractListModel
{
    Q_OBJECT
    ELA_VARIABLE(ElaNavigationNode*, SelectedNode)
public:
    explicit ElaFooterModel(QObject* parent = nullptr);
    ~ElaFooterModel();
    ElaNavigationType::NodeResult addFooterNode(const QString& footerTitle, QString& footerKey, bool isHasFooterPage, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None);
    int getFooterNodeCount() const;
    ElaNavigationNode* getNavigationNode(const QString& footerKey);
    void removeNavigationNode(const QString& footerKey);

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QList<ElaNavigationNode*> _footerNodeList;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAFOOTERMODEL_H_

