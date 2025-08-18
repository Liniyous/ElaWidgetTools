#ifndef ELAPIVOTSTYLE_H
#define ELAPIVOTSTYLE_H

#include <QModelIndex>
#include <QProxyStyle>

#include "ElaDef.h"
class ElaPivotStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PRIVATE_CREATE(QModelIndex, PressIndex)
    Q_PRIVATE_CREATE(int, CurrentIndex)
    Q_PRIVATE_CREATE(int, PivotSpacing)
public:
    explicit ElaPivotStyle(QStyle* style = nullptr);
    ~ElaPivotStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

    const QColor& getMarkColor();

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAPIVOTSTYLE_H
