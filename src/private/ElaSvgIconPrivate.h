#ifndef ELASVGICONPRIVATE_H
#define ELASVGICONPRIVATE_H

#include <QObject>
#include <QMap>
#include <QSvgRenderer>
#include <QPixmap>

#include "Def.h"
#include "stdafx.h"
class ElaSvgIcon;
class ElaSvgIconPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaSvgIcon)
public:
    explicit ElaSvgIconPrivate(QObject* parent = nullptr);
    ~ElaSvgIconPrivate();

private:
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
    QMap<SvgIconType::IconName, QString> _svgMap;
    SvgIconType::IconName _iconName = SvgIconType::NONE;

    QPixmap _pixmap;
    void updateIcon(SvgIconType::IconName icon = SvgIconType::NONE, int iconwidget=0, int iconheight=0);
};

#endif // ELASVGICONPRIVATE_H
