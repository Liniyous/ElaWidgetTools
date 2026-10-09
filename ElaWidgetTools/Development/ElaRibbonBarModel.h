#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELARIBBONBARMODEL_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELARIBBONBARMODEL_H_

#include "ElaWidgetToolsDef.h"
#include <QAbstractListModel>
#include <QAction>

struct ElaRibbonGroupData {
    QString groupName{""};
    QList<QAction*> ribbonDataList;
};

class ElaRibbonBarModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ElaRibbonBarModel(QObject* parent = nullptr);
    ~ElaRibbonBarModel() override;

    QAction* addRibbonAction(const QString& actionName);
    QAction* addRibbonAction(const QString& actionName, ElaIconType::IconName icon);
    QAction* addRibbonAction(const QString& actionName, const QIcon& icon);
    QAction* addRibbonAction(const QString& groupName, const QString& actionName);
    QAction* addRibbonAction(const QString& groupName, const QString& actionName, ElaIconType::IconName icon);
    QAction* addRibbonAction(const QString& groupName, const QString& actionName, const QIcon& icon);

    void removeRibbonAction(QAction* action);
    void removeRibbonAction(const QString& groupName, const QString& actionName);

    const QList<ElaRibbonGroupData>& getRibbonGroupData() const;
    QAction* getRibbonAction(int index) const;
    QModelIndex getRibbonActionModelIndex(QAction* ribbonAction) const;

    QList<QAction*> getRibbonActions();
    QList<QAction*> getRibbonActions(const QString& groupName);
    QList<QAction*> getRibbonActions(const QString& groupName, const QString& actionName);

protected:
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    void _watchAction(QAction* action);
    QList<ElaRibbonGroupData> _ribbonDataList;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELARIBBONBARMODEL_H_
