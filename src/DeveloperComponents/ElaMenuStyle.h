#ifndef ELAMENUSTYLE_H
#define ELAMENUSTYLE_H

#include <QProxyStyle>

#include "Def.h"
class ElaMenuStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaMenuStyle(QStyle* style = nullptr);
    ~ElaMenuStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);

private:
    int _shadowBorderWidth{6};
    ElaApplicationType::ThemeMode _themeMode;
    QLinearGradient* _windowLinearGradient{nullptr};
    int _iconLeftPadding{10};
    int _iconWidth{22};
    int _menuItemHeight{35};
    int _textLeftSpacing{15};
};

#endif // ELAMENUSTYLE_H
