#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELARADIOBUTTONSTYLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELARADIOBUTTONSTYLE_H_

#include <QPointer>
#include <QProxyStyle>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
class QPropertyAnimation;
class ElaRadioButtonStyle : public QProxyStyle
{
    Q_OBJECT
    // 动画保存半径比例，字体或指示器尺寸变化时仍按当前绘制区域缩放。
    ELA_PROPERTY(qreal, CircleScale)
public:
    explicit ElaRadioButtonStyle(QStyle* style = nullptr);
    ~ElaRadioButtonStyle() override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    QPropertyAnimation* _circleScaleAnimation{nullptr};
    QPointer<QWidget> _animationWidget;
    qreal _targetCircleScale{2.0 / 3.8};

    void _updateCircleScale(QStyle::State state, const QWidget* widget);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELARADIOBUTTONSTYLE_H_
