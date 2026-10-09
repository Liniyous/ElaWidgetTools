#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAMESSAGEBUTTON_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAMESSAGEBUTTON_H_

#include <QPushButton>
#include <QWidget>

#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaMessageButtonPrivate;
class ELA_EXPORT ElaMessageButton : public QPushButton
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaMessageButton)
    ELA_PROPERTY_Q_H(int, BorderRadius)
    ELA_PROPERTY_Q_H(QString, BarTitle);
    ELA_PROPERTY_Q_H(QString, BarText);
    ELA_PROPERTY_Q_H(int, DisplayMsec);
    ELA_PROPERTY_Q_H(QWidget*, MessageTargetWidget)
    ELA_PROPERTY_Q_H(ElaMessageBarType::MessageMode, MessageMode);
    ELA_PROPERTY_Q_H(ElaMessageBarType::PositionPolicy, PositionPolicy);

public:
    explicit ElaMessageButton(QWidget* parent = nullptr);
    explicit ElaMessageButton(const QString& text, QWidget* parent = nullptr);
    ~ElaMessageButton();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAMESSAGEBUTTON_H_
