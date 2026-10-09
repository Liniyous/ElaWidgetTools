#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATOGGLESWITCH_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATOGGLESWITCH_H_

#include <QWidget>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaToggleSwitchPrivate;
class ELA_EXPORT ElaToggleSwitch : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaToggleSwitch);

public:
    explicit ElaToggleSwitch(QWidget* parent = nullptr);
    ~ElaToggleSwitch() override;
    void setIsToggled(bool isToggled);
    bool getIsToggled() const;
Q_SIGNALS:
    Q_SIGNAL void toggled(bool checked);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELATOGGLESWITCH_H_
