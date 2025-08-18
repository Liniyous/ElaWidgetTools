#ifndef ELASTATUSBARSTYLE_H
#define ELASTATUSBARSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaStatusBarStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaStatusBarStyle(QStyle* style = nullptr);
    ~ElaStatusBarStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELASTATUSBARSTYLE_H
