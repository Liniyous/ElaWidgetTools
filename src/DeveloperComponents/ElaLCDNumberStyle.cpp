#include "ElaLCDNumberStyle.h"
#include "ElaTheme.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>
ElaLCDNumberStyle::ElaLCDNumberStyle(QStyle* style)
{
    _pIsTransparent = false;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaLCDNumberStyle::~ElaLCDNumberStyle()
{
}

void ElaLCDNumberStyle::drawControl(QStyle::ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
        {
            if (!_pIsTransparent)
            {
                // 背景绘制
                QRect numberRect = option->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing);
                painter->setPen(Qt::NoPen);
                // 边框绘制
                painter->setBrush(ElaThemeColor(_themeMode, BasicBorder));
                painter->drawRoundedRect(numberRect, 6, 6);

                painter->setBrush(ElaThemeColor(_themeMode, BasicBase));
                painter->drawRoundedRect(QRectF(numberRect.x() + 1.5, numberRect.y() + 1.5, numberRect.width() - 3, numberRect.height() - 3), 6, 6);
                painter->restore();
            }
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}
