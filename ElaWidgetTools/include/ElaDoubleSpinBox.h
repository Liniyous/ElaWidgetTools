#ifndef ELADOUBLESPINBOX_H
#define ELADOUBLESPINBOX_H

#include <QDoubleSpinBox>

#include "stdafx.h"

class ElaDoubleSpinBoxPrivate;
class ELA_EXPORT ElaDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
    Q_Q_CREATE(ElaDoubleSpinBox)
public:
    explicit ElaDoubleSpinBox(QWidget* parent = nullptr);
    ~ElaDoubleSpinBox();

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // ELADOUBLESPINBOX_H
