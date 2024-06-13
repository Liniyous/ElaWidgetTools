#include "ElaRadioButtonStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaApplication.h"
ElaRadioButtonStyle::ElaRadioButtonStyle(QStyle* style)
{
    _themeMode = ElaApplication::getInstance()->getThemeMode();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { _themeMode = themeMode; });
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
            painter->setPen(QPen(QColor(0x94, 0x94, 0x94), 1.5));
            if (bopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(QColor(0xDE, 0xDE, 0xDE));
            }
            else
            {
                painter->setBrush(Qt::white);
            }
            painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), 8.5, 8.5);
        }
        else
        {
            painter->setPen(Qt::NoPen);
            // 外圆形
            painter->setBrush(QColor(0x00, 0x66, 0xB4));
            painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 2, buttonRect.width() / 2);
            // 内圆形
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(0xEA, 0xEA, 0xEB));
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
