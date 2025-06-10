#ifndef ELADOUBLESPINBOX_H
#define ELADOUBLESPINBOX_H

#include <QDoubleSpinBox>

#include "Def.h"

class ElaDoubleSpinBoxPrivate;
class ELA_EXPORT ElaDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
    Q_Q_CREATE(ElaDoubleSpinBox)
    Q_PROPERTY_CREATE_Q_H(ElaSpinBoxType::ButtonMode, ButtonMode)
public:
    explicit ElaDoubleSpinBox(QWidget* parent = nullptr);
    ~ElaDoubleSpinBox() override;

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // ELADOUBLESPINBOX_H
