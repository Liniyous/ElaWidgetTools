#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAMASKWIDGET_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAMASKWIDGET_H_

#include <QWidget>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaMaskWidget : public QWidget
{
    Q_OBJECT
    ELA_PROPERTY(int, MaskAlpha)
public:
    explicit ElaMaskWidget(QWidget* parent = nullptr);
    ~ElaMaskWidget();
    void doMaskAnimation(int endValue);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAMASKWIDGET_H_

