#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELABREADCRUMBBAR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELABREADCRUMBBAR_H_
#include <QWidget>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaBreadcrumbBarPrivate;
class ELA_EXPORT ElaBreadcrumbBar : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaBreadcrumbBar)
    ELA_PROPERTY_Q_H(bool, IsAutoRemove)
public:
    explicit ElaBreadcrumbBar(QWidget* parent = nullptr);
    ~ElaBreadcrumbBar() override;

    void setTextPixelSize(int textPixelSize);
    Q_REQUIRED_RESULT int getTextPixelSize() const;

    void setBreadcrumbList(const QStringList& breadcrumbList);
    QStringList appendBreadcrumb(QString breadcrumb);
    QStringList removeBreadcrumb(QString breadcrumb);

    int getBreadcrumbListCount() const;
    const QStringList& getBreadcrumbList() const;

Q_SIGNALS:
    Q_SIGNAL void pTextPixelSizeChanged();
    Q_SIGNAL void breadcrumbClicked(QString breadcrumb, QStringList lastBreadcrumbList);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELABREADCRUMBBAR_H_
