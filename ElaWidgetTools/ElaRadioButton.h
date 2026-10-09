#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELARADIOBUTTON_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELARADIOBUTTON_H_

#include <QRadioButton>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaRadioButtonPrivate;
class ELA_EXPORT ElaRadioButton : public QRadioButton
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaRadioButton)
public:
    explicit ElaRadioButton(QWidget* parent = nullptr);
    explicit ElaRadioButton(const QString& text, QWidget* parent = nullptr);
    ~ElaRadioButton() override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELARADIOBUTTON_H_
