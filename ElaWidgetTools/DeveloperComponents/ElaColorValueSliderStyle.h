#ifndef ELACOLORVALUESLIDERSTYLE_H
#define ELACOLORVALUESLIDERSTYLE_H

#include <QLinearGradient>
#include <QProxyStyle>

#include "ElaDef.h"
class ElaColorValueSliderStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PRIVATE_CREATE(QColor, BaseColor)
public:
    explicit ElaColorValueSliderStyle(QStyle* style = nullptr);
    ~ElaColorValueSliderStyle();
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    QLinearGradient* _baseGradient{nullptr};
    mutable QStyle::State _lastState{QStyle::State_None};
    mutable qreal _circleRadius{0};
    void _startRadiusAnimation(qreal startRadius, qreal endRadius, QWidget* widget) const;
};

#endif // ELACOLORVALUESLIDERSTYLE_H
