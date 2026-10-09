#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLPAGEAREA_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLPAGEAREA_H_

#include <QWidget>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaScrollPageAreaPrivate;
class ELA_EXPORT ElaScrollPageArea : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaScrollPageArea)
    ELA_PROPERTY_Q_H(int, BorderRadius)
public:
    explicit ElaScrollPageArea(QWidget* parent = nullptr);
    ~ElaScrollPageArea() override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLPAGEAREA_H_
