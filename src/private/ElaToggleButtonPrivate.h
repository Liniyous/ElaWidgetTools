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
public:
    explicit ElaToggleButtonPrivate(QObject* parent = nullptr);
    ~ElaToggleButtonPrivate();

private:
    bool _isToggled{false};
    bool _isPressed{false};
    int _shadowBorderWidth{3};
    ElaApplicationType::ThemeMode _themeMode;
    void _initStyle();
};

#endif // ELATOGGLEBUTTONPRIVATE_H
