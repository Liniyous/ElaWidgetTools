#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAPIVOTMODEL_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAPIVOTMODEL_H_

#include <QAbstractListModel>
#include <QStringList>

class ElaPivotModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ElaPivotModel(QObject* parent = nullptr);
    ~ElaPivotModel();

    void appendPivot(const QString& pivot);
    void removePivot(const QString& pivot);
    void clearPivot();

    int getPivotListCount() const;
    const QStringList& getPivotList() const;
    int getPivotIndex(const QString& pivotTitle) const;

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QStringList _pivotList;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAPIVOTMODEL_H_

