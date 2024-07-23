#ifndef ELASCROLLBARSTYLE_H
#define ELASCROLLBARSTYLE_H
#include <QProxyStyle>

#include "Def.h"
class ElaScrollBarStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaScrollBarStyle(QStyle* style = nullptr);
    ~ElaScrollBarStyle();
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELASCROLLBARSTYLE_H
