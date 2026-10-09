#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAPIVOTSTYLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAPIVOTSTYLE_H_

#include "ElaPropertyMacro.h"
#include <QModelIndex>
#include <QProxyStyle>

#include "ElaWidgetToolsDef.h"
class ElaPivotStyle : public QProxyStyle
{
    Q_OBJECT
    ELA_VARIABLE(QModelIndex, PressIndex)
    ELA_VARIABLE(int, PivotSpacing)
public:
    explicit ElaPivotStyle(QStyle* style = nullptr);
    ~ElaPivotStyle() override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

    const QColor& getMarkColor();

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAPIVOTSTYLE_H_
