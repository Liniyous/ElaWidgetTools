#ifndef ELASVGICONPRIVATE_H
#define ELASVGICONPRIVATE_H

#include <QObject>
#include <QMap>

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
    QString getSvgPath(SvgIconType::IconName);
    QMap<SvgIconType::IconName, QString> _svgMap;
};

#endif // ELASVGICONPRIVATE_H
