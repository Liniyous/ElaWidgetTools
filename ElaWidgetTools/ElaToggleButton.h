#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATOGGLEBUTTON_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATOGGLEBUTTON_H_

#include <QPushButton>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaToggleButtonPrivate;
class ELA_EXPORT ElaToggleButton : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaToggleButton)
    ELA_PROPERTY_Q_H(int, BorderRadius)
    ELA_PROPERTY_Q_H(QString, Text)
public:
    explicit ElaToggleButton(QWidget* parent = nullptr);
    explicit ElaToggleButton(const QString& text, QWidget* parent = nullptr);
    ~ElaToggleButton();

    void setIsToggled(bool isToggled);
    bool getIsToggled() const;
Q_SIGNALS:
    Q_SIGNAL void toggled(bool checked);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELATOGGLEBUTTON_H_
