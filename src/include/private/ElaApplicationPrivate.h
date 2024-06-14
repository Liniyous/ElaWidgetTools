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
    Q_PROPERTY_CREATE_D(QColor, LightShadowEffectColor)
    Q_PROPERTY_CREATE_D(QColor, DarkShadowEffectColor)
    Q_PRIVATE_CREATE_D(QIcon, WindowIcon)
public:
    explicit ElaApplicationPrivate(QObject* parent = nullptr);
    ~ElaApplicationPrivate();

private:
    ElaApplicationType::ThemeMode _themeMode{ElaApplicationType::Light};
    void _init();
};

#endif // ELAAPPLICATIONPRIVATE_H
