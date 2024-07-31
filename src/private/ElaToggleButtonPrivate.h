#ifndef ELATOGGLEBUTTONPRIVATE_H
#define ELATOGGLEBUTTONPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaToggleButton;
class ElaToggleButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaToggleButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE(int, ToggleAlpha)
public:
    explicit ElaToggleButtonPrivate(QObject* parent = nullptr);
    ~ElaToggleButtonPrivate();

private:
    bool _isAlphaAnimationFinished{true};
    bool _isToggled{false};
    bool _isPressed{false};
    int _shadowBorderWidth{3};
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELATOGGLEBUTTONPRIVATE_H
