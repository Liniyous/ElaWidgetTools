#include "ElaScrollPageAreaPrivate.h"
#include "ElaTheme.h"

ElaScrollPageAreaPrivate::ElaScrollPageAreaPrivate(QObject *parent)
    : QObject{parent}
{}

ElaScrollPageAreaPrivate::~ElaScrollPageAreaPrivate()
{

}

QString ElaScrollPageAreaPrivate::colorToRgbaString(const QColor &color)
{
    return QString("rgba(%1, %2, %3, %4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alphaF(), 0, 'f', 2);
}

QString ElaScrollPageAreaPrivate::getStyleStr()
{
    return QString("ElaScrollPageArea{background-color:%1;border: 1px solid %2;"
                          "border-top-left-radius: %3px;border-top-right-radius: %4px;"
                          "border-bottom-left-radius: %5px;border-bottom-right-radius: %6px;}")
                      .arg(colorToRgbaString(ElaThemeColor(_themeMode, BasicBaseAlpha)))
                      .arg(eTheme->getThemeMode() == ElaThemeType::Light ? "rgba(0,0,0,15)" : "rgba(0,0,0,30)")
                      .arg(_topLeftRadius).arg(_topRightRadius)
                      .arg(_bottomLeftRadius).arg(_bottomRightRadius);
}
