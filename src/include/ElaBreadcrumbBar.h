#ifndef ELABREADCRUMBBAR_H
#define ELABREADCRUMBBAR_H
#include <QWidget>

#include "stdafx.h"
class ElaListView;
class ElaBreadcrumbBarModel;
class ElaBreadcrumbBarDelegate;
class ELA_EXPORT ElaBreadcrumbBar : public QWidget
{
    Q_OBJECT
public:
    explicit ElaBreadcrumbBar(QWidget* parent = nullptr);
    ~ElaBreadcrumbBar();
    void setBreadcrumbList(QStringList breadcrumbList);
    QStringList appendBreadcrumb(QString breadcrumb);
    QStringList removeBreadcrumb(QString breadcrumb);

    int getBreadcrumbListCount() const;
    QStringList getBreadcrumbList() const;

Q_SIGNALS:
    Q_SIGNAL void breadcrumbClicked(QString breadcrumb, QStringList lastBreadcrumbList);

private:
    ElaBreadcrumbBarModel* _listModel{nullptr};
    ElaBreadcrumbBarDelegate* _listDelegate{nullptr};
    ElaListView* _listView{nullptr};
};

#endif // ELABREADCRUMBBAR_H
