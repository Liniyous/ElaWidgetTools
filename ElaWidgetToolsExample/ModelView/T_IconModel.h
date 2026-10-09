#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_ICONMODEL_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_ICONMODEL_H_

#include <QAbstractListModel>
#include <QMetaEnum>

#include "ElaPropertyMacro.h"
class T_IconModel : public QAbstractListModel
{
    Q_OBJECT
    ELA_PROPERTY(bool, IsSearchMode)
public:
    explicit T_IconModel(QObject* parent = nullptr);
    ~T_IconModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    void setSearchKeyList(QStringList list);
    QStringList getSearchKeyList();

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QString getIconNameFromModelIndex(const QModelIndex& index) const;

private:
    QMetaEnum _metaEnum;
    QStringList _searchKeyList;
    int _rowCount;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_ICONMODEL_H_


