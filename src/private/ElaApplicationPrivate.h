#ifndef ELAAPPLICATIONPRIVATE_H
#define ELAAPPLICATIONPRIVATE_H

#include <QColor>
#include <QIcon>
#include <QObject>

#include "stdafx.h"
class ElaApplication;
class ElaApplicationPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaApplication)
    Q_PROPERTY_CREATE_D(bool, IsApplicationClosed)
public:
    explicit ElaApplicationPrivate(QObject* parent = nullptr);
    ~ElaApplicationPrivate();
};

#endif // ELAAPPLICATIONPRIVATE_H
