#ifndef ELAMENUSTYLE_H
#define ELAMENUSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"

class ElaMenuStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, MenuItemHeight);

public:
    explicit ElaMenuStyle(QStyle* style = nullptr);
    ~ElaMenuStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

private:
    mutable bool _isAnyoneItemHasIcon{false};
    int _shadowBorderWidth{6};
    ElaThemeType::ThemeMode _themeMode;
    mutable int _iconWidth{0};
};

#endif // ELAMENUSTYLE_H
