#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_LISTVIEWMODEL_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_LISTVIEWMODEL_H_

#include <QAbstractListModel>

class T_ListViewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit T_ListViewModel(QObject* parent = nullptr);
    ~T_ListViewModel();

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QStringList _dataList;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_LISTVIEWMODEL_H_

