#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOLORVALUESLIDERSTYLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOLORVALUESLIDERSTYLE_H_

#include "ElaPropertyMacro.h"
#include <QLinearGradient>
#include <QProxyStyle>

#include "ElaWidgetToolsDef.h"
class ElaColorValueSliderStyle : public QProxyStyle
{
    Q_OBJECT
    ELA_VARIABLE(QColor, BaseColor)
public:
    explicit ElaColorValueSliderStyle(QStyle* style = nullptr);
    ~ElaColorValueSliderStyle();
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    mutable QLinearGradient _baseGradient{0, 0, 100, 100}; // 颜色滑槽的明度渐变
    mutable QStyle::State _lastState{QStyle::State_None};
    mutable qreal _circleRadius{0};
    void _startRadiusAnimation(qreal startRadius, qreal endRadius, QWidget* widget) const;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOLORVALUESLIDERSTYLE_H_
