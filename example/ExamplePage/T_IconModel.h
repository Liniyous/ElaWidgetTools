#ifndef T_ICONMODEL_H
#define T_ICONMODEL_H

#include <QAbstractTableModel>
#include <QMetaEnum>

#include "stdafx.h"
class T_IconModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY_CREATE(bool, IsSearchMode)
public:
    explicit T_IconModel(QObject* parent = nullptr);
    ~T_IconModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    void setSearchKeyList(QStringList list);
    QStringList getSearchKeyList();
    void setColumnCount(int count);

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    QMetaEnum _metaEnum;
    QStringList _searchKeyList;
    int _rowCount;
    int _columnCount;
};

#endif // T_ICONMODEL_H
