#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_TABLEVIEWMODEL_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_TABLEVIEWMODEL_H_

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

class T_TableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit T_TableViewModel(QObject* parent = nullptr);
    ~T_TableViewModel() override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    QStringList _header;
    QList<QStringList> _dataList;
    QList<QIcon> _iconList;
};

// 音乐资料库筛选与排序代理模型
class T_MusicFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit T_MusicFilterModel(QObject* parent = nullptr);
    ~T_MusicFilterModel() override;

protected:
    bool lessThan(const QModelIndex& sourceLeft, const QModelIndex& sourceRight) const override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_TABLEVIEWMODEL_H_
