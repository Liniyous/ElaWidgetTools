#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAACTIONCOMMANDERVIEWSTYLE_H
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAACTIONCOMMANDERVIEWSTYLE_H

#include "ElaPropertyMacro.h"
#include <QProxyStyle>

#include "ElaWidgetToolsDef.h"

class ElaActionCommanderViewStyle : public QProxyStyle
{
    Q_OBJECT
    ELA_PROPERTY(int, ItemHeight)
    ELA_PROPERTY(int, HeaderMargin)
public:
    explicit ElaActionCommanderViewStyle(QStyle* style = nullptr);
    ~ElaActionCommanderViewStyle() override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;
    QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const override;

    ElaThemeType::ThemeMode getThemeMode() const;

private:
    ElaThemeType::ThemeMode _themeMode;
    int _leftPadding{11};
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAACTIONCOMMANDERVIEWSTYLE_H
