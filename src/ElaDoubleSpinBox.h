#ifndef ELADOUBLESPINBOX_H
#define ELADOUBLESPINBOX_H

#include <QDoubleSpinBox>

class ElaDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit ElaDoubleSpinBox(QWidget* parent = nullptr);
    ~ElaDoubleSpinBox();
};

#endif // ELADOUBLESPINBOX_H
