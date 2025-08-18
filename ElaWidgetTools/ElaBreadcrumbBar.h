#ifndef ELABREADCRUMBBAR_H
#define ELABREADCRUMBBAR_H
#include <QWidget>

#include "ElaProperty.h"

class ElaBreadcrumbBarPrivate;
class ELA_EXPORT ElaBreadcrumbBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaBreadcrumbBar)
    Q_PROPERTY_CREATE_Q_H(int, TextPixelSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsAutoRemove)
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
};

#endif // ELABREADCRUMBBAR_H
