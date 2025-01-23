#ifndef ELASPINBOXPRIVATE_H
#define ELASPINBOXPRIVATE_H

#include <QObject>

#include "stdafx.h"
class ElaSpinBox;
class ElaMenu;
class ElaSpinBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaSpinBox)
public:
    explicit ElaSpinBoxPrivate(QObject* parent = nullptr);
    ~ElaSpinBoxPrivate();

private:
    ElaMenu* _createStandardContextMenu();
};

#endif // ELASPINBOXPRIVATE_H
