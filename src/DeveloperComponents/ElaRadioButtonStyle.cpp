#include "ElaRadioButtonStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaTheme.h"
ElaRadioButtonStyle::ElaRadioButtonStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
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
        QRect buttonRect = bopt->rect;
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        if (bopt->state & QStyle::State_Off)
        {
            painter->setPen(QPen(ElaThemeColor(_themeMode, RadioButtonStateOffBorder), 1.5));
            if (bopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(ElaThemeColor(_themeMode, RadioButtonStateOffHover));
            }
            else
            {
                painter->setBrush(ElaThemeColor(_themeMode, RadioButtonStateOffBase));
            }
            painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), 8.5, 8.5);
        }
        else
        {
            painter->setPen(ElaThemeColor(_themeMode, RadioButtonStateOnBorder));
            // 外圆形
            painter->setBrush(ElaThemeColor(_themeMode, RadioButtonStateOnBase));
            painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 2, buttonRect.width() / 2);
            // 内圆形
            painter->setPen(Qt::NoPen);
            painter->setBrush(ElaThemeColor(_themeMode, RadioButtonStateOnCenter));
            if (bopt->state & QStyle::State_Sunken)
            {
                if (bopt->state & QStyle::State_MouseOver)
                {
                    painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 4.5, buttonRect.width() / 4.5);
                }
            }
            else
            {
                if (bopt->state & QStyle::State_MouseOver)
                {
                    painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 3.5, buttonRect.width() / 3.5);
                }
                else
                {
                    painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 4, buttonRect.width() / 4);
                }
            }
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
    {
        return 20;
    }
    case QStyle::PM_ExclusiveIndicatorHeight:
    {
        return 20;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}
