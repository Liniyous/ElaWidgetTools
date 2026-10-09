#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELASCROLLBARSTYLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELASCROLLBARSTYLE_H_
#include "ElaPropertyMacro.h"
#include <QProxyStyle>

#include "ElaWidgetToolsDef.h"
class ElaScrollBar;
class ElaScrollBarStyle : public QProxyStyle
{
    Q_OBJECT
    ELA_VARIABLE(bool, IsExpand)
    ELA_PROPERTY(qreal, Opacity)
    ELA_PROPERTY(qreal, SliderExtent)
    ELA_VARIABLE(ElaScrollBar*, ScrollBar)
public:
    explicit ElaScrollBarStyle(QStyle* style = nullptr);
    ~ElaScrollBarStyle();
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;
    void startExpandAnimation(bool isExpand);

private:
    ElaThemeType::ThemeMode _themeMode;
    qreal _sliderMargin{2.5};
    int _scrollBarExtent{10};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELASCROLLBARSTYLE_H_

