#include "ElaPlainTextEditStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "ElaTheme.h"
ElaPlainTextEditStyle::ElaPlainTextEditStyle(QStyle* style)
{
    _pExpandMarkWidth = 0;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaPlainTextEditStyle::~ElaPlainTextEditStyle()
{
}

void ElaPlainTextEditStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
        {
            //背景绘制
            QRect editRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            // 边框绘制
            painter->setBrush(ElaThemeColor(_themeMode, BasicBorder));
            painter->drawRoundedRect(editRect, 6, 6);

            painter->setBrush(ElaThemeColor(_themeMode, BasicBase));
            painter->drawRoundedRect(QRectF(editRect.x() + 1.5, editRect.y() + 1.5, editRect.width() - 3, editRect.height() - 3), 6, 6);

            // 底边线绘制
            painter->setBrush(ElaThemeColor(_themeMode, BasicHemline));
            QPainterPath path;
            int l = 12;
            int r = l / 2;
            path.moveTo(l, editRect.height() - r + (r * 0.70710678118654752440084436210485));
            path.arcTo(QRect(0, editRect.height() - l, l, l), 225, 45);
            path.arcTo(QRect(editRect.width() - l, editRect.height() - l, l, l), 270, 45);
            path.closeSubpath();
            painter->fillPath(path, ElaThemeColor(_themeMode, BasicHemline));

            //焦点指示器
            painter->setPen(Qt::NoPen);
            painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
            painter->drawRoundedRect(QRectF(editRect.width() / 2 - _pExpandMarkWidth, editRect.height() - 2.5, _pExpandMarkWidth * 2, 2.5), 2, 2);

            painter->restore();
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
