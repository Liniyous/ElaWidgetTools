#include "ElaRadioButtonStyle.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QStyleOption>
#include <QWidget>

#include "ElaTheme.h"
ElaRadioButtonStyle::ElaRadioButtonStyle(QStyle* style)
{
    _pCircleScale = 2.0 / 4.0;
    _circleScaleAnimation = new QPropertyAnimation(this, "pCircleScale", this);
    _circleScaleAnimation->setDuration(200);
    _circleScaleAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(this, &ElaRadioButtonStyle::pCircleScaleChanged, this, [this]() {
        if (_animationWidget)
        {
            _animationWidget->update();
        }
    });
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaRadioButtonStyle::~ElaRadioButtonStyle()
{
}

void ElaRadioButtonStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case PE_IndicatorRadioButton:
    {
        const QStyleOptionButton* bopt = qstyleoption_cast<const QStyleOptionButton*>(option);
        if (!bopt)
        {
            break;
        }
        QRectF buttonRect = bopt->rect;
        buttonRect.adjust(1, 1, -1, -1);
        const QPointF center = buttonRect.center();
        const qreal radius = qMax<qreal>(0, qMin(buttonRect.width(), buttonRect.height()) / 2);
        const_cast<ElaRadioButtonStyle*>(this)->_updateCircleScale(bopt->state, widget);
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        if (bopt->state & QStyle::State_Off)
        {
            painter->setPen(QPen(ElaThemeColor(_themeMode, BasicBorder), 1.5));
            if (bopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(ElaThemeColor(_themeMode, BasicHover));
            }
            else
            {
                painter->setBrush(ElaThemeColor(_themeMode, BasicBase));
            }
            const qreal borderRadius = qMax<qreal>(0, radius - 0.75);
            painter->drawEllipse(center, borderRadius, borderRadius);
        }
        else
        {
            painter->setPen(Qt::NoPen);
            // 外圆形
            painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
            painter->drawEllipse(center, radius, radius);
            // 内圆形
            painter->setBrush(ElaThemeColor(_themeMode, BasicTextInvert));
            const qreal circleRadius = radius * _pCircleScale;
            painter->drawEllipse(center, circleRadius, circleRadius);
        }
        painter->restore();
        return;
    }
    default:
    {
        break;
    }
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int ElaRadioButtonStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_ExclusiveIndicatorWidth:
    case QStyle::PM_ExclusiveIndicatorHeight:
    {
        return widget ? widget->fontMetrics().height() : QProxyStyle::pixelMetric(metric, option, widget);
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

void ElaRadioButtonStyle::_updateCircleScale(QStyle::State state, const QWidget* widget)
{
    qreal targetScale = 2.0 / 3.8;
    if (state.testFlag(QStyle::State_Enabled) && state.testFlag(QStyle::State_On))
    {
        if (state.testFlag(QStyle::State_Sunken))
        {
            targetScale = 2.0 / 4.5;
        }
        else if (state.testFlag(QStyle::State_MouseOver))
        {
            targetScale = 2.0 / 3.0;
        }
    }
    if (!widget || !widget->isVisible() || !state.testFlag(QStyle::State_Enabled) || !state.testFlag(QStyle::State_On))
    {
        _circleScaleAnimation->stop();
        _targetCircleScale = targetScale;
        _animationWidget.clear();
        if (!qFuzzyCompare(_pCircleScale, targetScale))
        {
            setCircleScale(targetScale);
        }
        return;
    }
    _animationWidget = const_cast<QWidget*>(widget);
    if (qFuzzyCompare(_targetCircleScale, targetScale))
    {
        return;
    }
    _targetCircleScale = targetScale;
    // 复用同一个动画，并从当前比例续接，快速切换悬停或按下状态时不会跳变。
    _circleScaleAnimation->stop();
    _circleScaleAnimation->setStartValue(_pCircleScale);
    _circleScaleAnimation->setEndValue(targetScale);
    _circleScaleAnimation->start();
}
