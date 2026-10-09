#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELADOUBLESPINBOX_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELADOUBLESPINBOX_H_

#include "ElaPropertyMacro.h"
#include <QDoubleSpinBox>

#include "ElaWidgetToolsDef.h"

class ElaDoubleSpinBoxPrivate;
class ELA_EXPORT ElaDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaDoubleSpinBox)
public:
    explicit ElaDoubleSpinBox(QWidget* parent = nullptr);
    ~ElaDoubleSpinBox() override;

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

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELADOUBLESPINBOX_H_
