#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATOOLTIP_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATOOLTIP_H_

#include <QWidget>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"
class ElaToolTipPrivate;
class ELA_EXPORT ElaToolTip : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaToolTip)
    ELA_PROPERTY_Q_H(int, BorderRadius)
    ELA_PROPERTY_Q_H(int, DisplayMsec)
    ELA_PROPERTY_Q_H(int, ShowDelayMsec)
    ELA_PROPERTY_Q_H(int, HideDelayMsec)
public:
    explicit ElaToolTip(QWidget* parent = nullptr);
    ~ElaToolTip() override;

    void setToolTip(QString toolTip);
    Q_REQUIRED_RESULT QString getToolTip() const;
    void setCustomWidget(QWidget* customWidget);
    Q_REQUIRED_RESULT QWidget* getCustomWidget() const;

    void updatePos();

protected:
    virtual void paintEvent(QPaintEvent* event);

Q_SIGNALS:
    Q_SIGNAL void pToolTipChanged();
    Q_SIGNAL void pCustomWidgetChanged();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELATOOLTIP_H_
