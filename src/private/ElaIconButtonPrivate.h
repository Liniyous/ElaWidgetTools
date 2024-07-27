#ifndef ELAICONBUTTONPRIVATE_H
#define ELAICONBUTTONPRIVATE_H

#include <QColor>
#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaIconButton;
class ElaIconButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaIconButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(ElaIconType, Awesome)
    Q_PROPERTY_CREATE_D(QColor, LightHoverColor);
    Q_PROPERTY_CREATE_D(QColor, DarkHoverColor);
    Q_PROPERTY_CREATE_D(QColor, LightIconColor);
    Q_PROPERTY_CREATE_D(QColor, DarkIconColor);
    Q_PROPERTY_CREATE_D(bool, IsSelected);
    Q_PROPERTY_CREATE(int, HoverAlpha)
public:
    explicit ElaIconButtonPrivate(QObject* parent = nullptr);
    ~ElaIconButtonPrivate();

private:
    bool _isAlphaAnimationFinished{true};
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAICONBUTTONPRIVATE_H
