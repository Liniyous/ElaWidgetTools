#ifndef ELAAPPLICATION_H
#define ELAAPPLICATION_H

#include <QColor>
#include <QIcon>
#include <QObject>

#include "Def.h"
#include "singleton.h"
#include "stdafx.h"
class ELA_EXPORT ElaApplication : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE(ElaApplication)
    Q_PROPERTY_CREATE(bool, IsApplicationClosed)
    Q_PROPERTY_CREATE(QColor, ShadowEffectColor)
    Q_PRIVATE_CREATE(QIcon, WindowIcon)
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

private:
    ElaApplicationType::ThemeMode _themeMode{ElaApplicationType::Light};
};

#endif // ELAAPPLICATION_H
