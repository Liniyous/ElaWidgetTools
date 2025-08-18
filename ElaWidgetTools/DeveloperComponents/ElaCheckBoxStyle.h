#ifndef ELACHECKBOXSTYLE_H
#define ELACHECKBOXSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaCheckBoxStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PRIVATE_CREATE(int, CheckIndicatorWidth)
public:
    explicit ElaCheckBoxStyle(QStyle* style = nullptr);
    ~ElaCheckBoxStyle();
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELACHECKBOXSTYLE_H
