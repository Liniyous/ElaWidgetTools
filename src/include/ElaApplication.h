#ifndef ELAAPPLICATION_H
#define ELAAPPLICATION_H

#include <QColor>
#include <QIcon>
#include <QObject>

#include "Def.h"
#include "singleton.h"
#include "stdafx.h"
class ElaApplicationPrivate;
class ELA_EXPORT ElaApplication : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaApplication)
    Q_SINGLETON_CREATE_H(ElaApplication)
    Q_PROPERTY_CREATE_Q_H(bool, IsApplicationClosed)
    Q_PROPERTY_CREATE_Q_H(QColor, LightShadowEffectColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkShadowEffectColor)
    Q_PRIVATE_CREATE_Q_H(QIcon, WindowIcon)
private:
    explicit ElaApplication(QObject* parent = nullptr);
    ~ElaApplication();

public:
    void init();
    static bool containsCursorToItem(QWidget* item);
    void setThemeMode(ElaApplicationType::ThemeMode themeMode);
    ElaApplicationType::ThemeMode getThemeMode() const;
Q_SIGNALS:
    Q_SIGNAL void themeModeChanged(ElaApplicationType::ThemeMode themeMode);
};

#endif // ELAAPPLICATION_H
