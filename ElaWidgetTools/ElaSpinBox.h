#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELASPINBOX_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELASPINBOX_H_

#include "ElaPropertyMacro.h"
#include <QSpinBox>

#include "ElaWidgetToolsDef.h"

class ElaSpinBoxPrivate;
class ELA_EXPORT ElaSpinBox : public QSpinBox
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaSpinBox)
public:
    explicit ElaSpinBox(QWidget* parent = nullptr);
    ~ElaSpinBox() override;

    void setButtonMode(ElaSpinBoxType::ButtonMode buttonMode);
    Q_REQUIRED_RESULT ElaSpinBoxType::ButtonMode getButtonMode() const;

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

Q_SIGNALS:
    Q_SIGNAL void pButtonModeChanged();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELASPINBOX_H_
