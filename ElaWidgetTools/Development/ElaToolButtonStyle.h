#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELATOOLBUTTONSTYLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELATOOLBUTTONSTYLE_H_

#include "ElaPropertyMacro.h"
#include <QProxyStyle>

#include "ElaWidgetToolsDef.h"
class QStyleOptionToolButton;
class ElaControlAnimation;
class ElaToolButtonStyle : public QProxyStyle
{
    Q_OBJECT
    ELA_VARIABLE(bool, IsTransparent)
    ELA_VARIABLE(int, BorderRadius)
    ELA_VARIABLE(bool, IsSelected);
    ELA_VARIABLE(Qt::Alignment, TextAlignment)

public:
    explicit ElaToolButtonStyle(QStyle* style = nullptr);
    ~ElaToolButtonStyle() override;
    void setControlAnimation(ElaControlAnimation* animation);
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    int _contentMargin{6};
    ElaControlAnimation* _controlAnimation{nullptr};
    void _drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
    void _drawIcon(QPainter* painter, QRectF iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
    void _drawText(QPainter* painter, QRect contentRect, const QStyleOptionToolButton* bopt) const;
    int _getExpandIndicatorWidth(const QStyleOptionToolButton* bopt) const;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELATOOLBUTTONSTYLE_H_
