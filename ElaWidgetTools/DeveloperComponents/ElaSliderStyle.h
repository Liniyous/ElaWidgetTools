#ifndef ELASLIDERSTYLE_H
#define ELASLIDERSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaSliderStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaSliderStyle(QStyle* style = nullptr);
    ~ElaSliderStyle();
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    mutable QStyle::State _lastState{QStyle::State_None};
    mutable qreal _circleRadius{0};
    void _startRadiusAnimation(qreal startRadius, qreal endRadius, QWidget* widget) const;
};

#endif // ELASLIDERSTYLE_H
