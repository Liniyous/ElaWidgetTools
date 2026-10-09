#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_TREEVIEWMODEL_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_TREEVIEWMODEL_H_

#include <QAbstractItemModel>
#include <QMap>
#include <QObject>

class T_TreeItem;
class T_TreeViewModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit T_TreeViewModel(QObject* parent = nullptr);
    ~T_TreeViewModel();
    QModelIndex parent(const QModelIndex& child) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int getItemCount() const;

private:
    QMap<QString, T_TreeItem*> _itemsMap;
    T_TreeItem* _rootItem{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_TREEVIEWMODEL_H_

