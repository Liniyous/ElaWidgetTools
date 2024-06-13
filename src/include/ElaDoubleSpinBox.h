#ifndef ELADOUBLESPINBOX_H
#define ELADOUBLESPINBOX_H

#include <QDoubleSpinBox>

#include "stdafx.h"
class ELA_EXPORT ElaDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit ElaDoubleSpinBox(QWidget* parent = nullptr);
    ~ElaDoubleSpinBox();
};

#endif // ELADOUBLESPINBOX_H
