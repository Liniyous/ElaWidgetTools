#ifndef ELAMENUMODEL_H
#define ELAMENUMODEL_H

#include <QAbstractListModel>
class QAction;
class ElaMenuModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ElaMenuModel(QObject* parent = nullptr);
    ~ElaMenuModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    void appendAction(QAction* action);

private:
    QList<QAction*> _actionList;
};

#endif // ELAMENUMODEL_H
