#ifndef ELADOUBLESPINBOXPRIVATE_H
#define ELADOUBLESPINBOXPRIVATE_H

#include <QObject>

#include "stdafx.h"
class ElaMenu;
class ElaDoubleSpinBox;
class ElaDoubleSpinBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaDoubleSpinBox)
public:
    explicit ElaDoubleSpinBoxPrivate(QObject* parent = nullptr);
    ~ElaDoubleSpinBoxPrivate();

private:
    ElaMenu* _createStandardContextMenu();
};

#endif // ELADOUBLESPINBOXPRIVATE_H
