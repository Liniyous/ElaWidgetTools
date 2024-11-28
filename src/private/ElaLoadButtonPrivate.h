#ifndef ELALOADBUTTONPRIVATE_H
#define ELALOADBUTTONPRIVATE_H

#include <QColor>
#include <QObject>
#include <QLabel>
#include <QTimer>

#include "Def.h"
#include "stdafx.h"
class ElaProgressRing;
class ElaLoadButton;
class ElaLoadButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaLoadButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkPressColor)
public:
    explicit ElaLoadButtonPrivate(QObject* parent = nullptr);
    ~ElaLoadButtonPrivate();

private:
    QColor _lightTextColor;
    QColor _darkTextColor;
    bool _isPressed{false};
    int _shadowBorderWidth{3};
    ElaThemeType::ThemeMode _themeMode;
    QLabel* _text{nullptr};
    ElaProgressRing* _progressRing{nullptr};
    QTimer* _outTimer{nullptr};
};

#endif // ELALOADBUTTONPRIVATE_H
