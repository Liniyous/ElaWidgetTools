#ifndef ELABREADCRUMBBARMODEL_H
#define ELABREADCRUMBBARMODEL_H

#include <QAbstractListModel>

class ElaBreadcrumbBarModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ElaBreadcrumbBarModel(QObject* parent = nullptr);
    ~ElaBreadcrumbBarModel();

    void appendBreadcrumb(QString breadcrumb);
    void removeBreadcrumb(QString breadcrumb);
    void removeBreadcrumb(int index);

    void setBreadcrumbList(QStringList breadcrumbList);

    int getBreadcrumbListCount();
    QStringList getBreadcrumbList();

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QStringList _breadcrumbList;
};

#endif // ELABREADCRUMBBARMODEL_H
