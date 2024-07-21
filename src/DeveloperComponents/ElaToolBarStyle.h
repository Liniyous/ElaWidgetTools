#ifndef ELATOOLBARSTYLE_H
#define ELATOOLBARSTYLE_H

#include <QProxyStyle>

#include "Def.h"
class ElaToolBarStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaToolBarStyle(QStyle* style = nullptr);
    ~ElaToolBarStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    ElaApplicationType::ThemeMode _themeMode;
};

#endif // ELATOOLBARSTYLE_H
