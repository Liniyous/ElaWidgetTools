#ifndef ELATABBARSTYLE_H
#define ELATABBARSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaTabBarStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PRIVATE_CREATE(QSize, TabSize)
public:
    explicit ElaTabBarStyle(QStyle* style = nullptr);
    ~ElaTabBarStyle() override;
    void drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;
    QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELATABBARSTYLE_H
