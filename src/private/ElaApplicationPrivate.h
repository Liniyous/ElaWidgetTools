#ifndef ELAAPPLICATIONPRIVATE_H
#define ELAAPPLICATIONPRIVATE_H

#include <QColor>
#include <QIcon>
#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaApplication;
class ElaApplicationPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaApplication)
    Q_PROPERTY_CREATE_D(bool, IsApplicationClosed)
    Q_PRIVATE_CREATE_D(QIcon, WindowIcon)
public:
    explicit ElaApplicationPrivate(QObject* parent = nullptr);
    ~ElaApplicationPrivate();

private:
    void _init();
};

#endif // ELAAPPLICATIONPRIVATE_H
