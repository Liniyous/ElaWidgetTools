#include "ElaLineEditStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "ElaTheme.h"
ElaLineEditStyle::ElaLineEditStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaLineEditStyle::~ElaLineEditStyle()
{
}

void ElaLineEditStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case PE_PanelLineEdit:
    {
        if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
        {
            QRect lineEditRect = fopt->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            // 边框绘制
            painter->setBrush(ElaThemeColor(_themeMode, BasicBorder));
            painter->drawRoundedRect(lineEditRect, 6, 6);
            //  背景绘制
            if (fopt->state & QStyle::State_HasFocus)
            {
                painter->setBrush(ElaThemeColor(_themeMode, DialogBase));
            }
            else if (fopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(ElaThemeColor(_themeMode, BasicHover));
            }
            else
            {
                painter->setBrush(ElaThemeColor(_themeMode, BasicBase));
            }
            painter->drawRoundedRect(QRectF(lineEditRect.x() + 1.5, lineEditRect.y() + 1.5, lineEditRect.width() - 3, lineEditRect.height() - 3), 6, 6);

            // 底边线绘制
            QPainterPath path;
            int l = 12;
            int r = l / 2;
            path.moveTo(l, lineEditRect.height() - r + (r * 0.70710678118654752440084436210485));
            path.arcTo(QRect(0, lineEditRect.height() - l, l, l), 225, 45);
            path.arcTo(QRect(lineEditRect.width() - l, lineEditRect.height() - l, l, l), 270, 45);
            path.closeSubpath();
            painter->fillPath(path, ElaThemeColor(_themeMode, BasicHemline));
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
