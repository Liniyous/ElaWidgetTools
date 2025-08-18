#ifndef ELATREEVIEWSTYLE_H
#define ELATREEVIEWSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaTreeViewStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, ItemHeight)
    Q_PROPERTY_CREATE(int, HeaderMargin)
public:
    explicit ElaTreeViewStyle(QStyle* style = nullptr);
    ~ElaTreeViewStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;
    QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    int _leftPadding{11};
};

#endif // ELATREEVIEWSTYLE_H
