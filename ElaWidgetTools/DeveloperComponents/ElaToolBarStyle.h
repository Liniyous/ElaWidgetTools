#ifndef ELATOOLBARSTYLE_H
#define ELATOOLBARSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class QStyleOptionToolButton;
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
    ElaThemeType::ThemeMode _themeMode;
    void _drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
    void _drawIcon(QPainter* painter, QRectF iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
    void _drawText(QPainter* painter, QRect contentRect, const QStyleOptionToolButton* bopt) const;
};

#endif // ELATOOLBARSTYLE_H
