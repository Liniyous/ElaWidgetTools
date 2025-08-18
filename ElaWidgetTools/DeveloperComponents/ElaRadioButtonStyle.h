#ifndef ELARADIOBUTTONSTYLE_H
#define ELARADIOBUTTONSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaRadioButtonStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaRadioButtonStyle(QStyle* style = nullptr);
    ~ElaRadioButtonStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELARADIOBUTTONSTYLE_H
