#include "ElaSvgIconPrivate.h"

#include "ElaSvgIcon.h"
ElaSvgIconPrivate::ElaSvgIconPrivate(QObject* parent)
    : QObject{parent}
{
    _svgMap[SvgIconType::HOME] = "Home";
}

ElaSvgIconPrivate::~ElaSvgIconPrivate()
{
}

QString ElaSvgIconPrivate::getSvgPath(SvgIconType::IconName svg)
{
    QString str = QString(":/include/Image/icons/%1_%2.svg")
                      .arg(_svgMap[svg]).arg(_themeMode == ElaThemeType::Light ? "black" : "white");
    return str;
}
