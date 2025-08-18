#ifndef ELAMENUBARSTYLE_H
#define ELAMENUBARSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaMenuBarStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaMenuBarStyle(QStyle* style = nullptr);
    ~ElaMenuBarStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    int _menuBarItemMargin{0};
};

#endif // ELAMENUBARSTYLE_H
