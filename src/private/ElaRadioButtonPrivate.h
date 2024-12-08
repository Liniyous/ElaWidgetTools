#ifndef ELARADIOBUTTONPRIVATE_H
#define ELARADIOBUTTONPRIVATE_H
#include <QObject>

#include "Def.h"
class ElaRadioButton;
class ElaRadioButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaRadioButton)
public:
    explicit ElaRadioButtonPrivate(QObject* parent = nullptr);
    ~ElaRadioButtonPrivate();
};

#endif // ELARADIOBUTTONPRIVATE_H
