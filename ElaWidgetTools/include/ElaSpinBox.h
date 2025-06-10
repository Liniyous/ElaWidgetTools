#ifndef ELASPINBOX_H
#define ELASPINBOX_H

#include <QSpinBox>

#include "Def.h"

class ElaSpinBoxPrivate;
class ELA_EXPORT ElaSpinBox : public QSpinBox
{
    Q_OBJECT
    Q_Q_CREATE(ElaSpinBox)
    Q_PROPERTY_CREATE_Q_H(ElaSpinBoxType::ButtonMode, ButtonMode)
public:
    explicit ElaSpinBox(QWidget* parent = nullptr);
    ~ElaSpinBox() override;

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // ELASPINBOX_H
