#include "ElaScrollBarStyle.h"

#include <QPainter>
#include <QStyleOptionSlider>

#include "ElaTheme.h"
ElaScrollBarStyle::ElaScrollBarStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaScrollBarStyle::~ElaScrollBarStyle()
{
}

void ElaScrollBarStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
    case QStyle::CC_ScrollBar:
    {
        const QStyleOptionSlider* sopt = qstyleoption_cast<const QStyleOptionSlider*>(option);
        if (nullptr == sopt)
        {
            break;
        }
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::transparent);
        painter->drawRoundedRect(sopt->rect, 3, 3);
        QRect sliderRect = subControlRect(control, sopt, SC_ScrollBarSlider, widget);
        if (sopt->orientation == Qt::Horizontal)
        {
            sliderRect.setRect(sliderRect.x() - 6, sliderRect.y(), sliderRect.width() + 12, sliderRect.height());
        }
        else
        {
            sliderRect.setRect(sliderRect.x(), sliderRect.y() - 6, sliderRect.width(), sliderRect.height() + 12);
        }
        if ((sopt->orientation == Qt::Horizontal && sopt->rect.width() != sliderRect.width()) || (sopt->orientation == Qt::Vertical && sopt->rect.height() != sliderRect.height()))
        {
            painter->setBrush(ElaThemeColor(_themeMode, ScrollBarHandle));
            if (sopt->state & QStyle::State_MouseOver)
            {
                painter->drawRoundedRect(sliderRect, 3, 3);
            }
            else
            {
                if (sopt->orientation == Qt::Horizontal)
                {
                    sliderRect.setRect(sliderRect.x(), sliderRect.y() + 1, sliderRect.width(), sliderRect.height() - 2);
                }
                else
                {
                    sliderRect.setRect(sliderRect.x() + 1, sliderRect.y(), sliderRect.width() - 2, sliderRect.height());
                }
                painter->drawRoundedRect(sliderRect, 2, 2);
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

    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

int ElaScrollBarStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_ScrollBarExtent:
    {
        return 6;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}
