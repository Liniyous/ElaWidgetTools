#ifndef ELATHEMEPRIVATE_H
#define ELATHEMEPRIVATE_H

#include <QColor>
#include <QMap>
#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaTheme;
class ElaThemePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTheme)
public:
    explicit ElaThemePrivate(QObject* parent = nullptr);
    ~ElaThemePrivate();

private:
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
    QColor _lightThemeColorList[174];
    QColor _darkThemeColorList[174];
    void _initThemeColor();
};

#endif // ELATHEMEPRIVATE_H
