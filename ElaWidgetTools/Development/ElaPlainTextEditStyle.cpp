#include "ElaPlainTextEditStyle.h"

#include "ElaControlStyle.h"
#include "ElaTheme.h"

#include <QPainter>
#include <QStyleOption>

ElaPlainTextEditStyle::ElaPlainTextEditStyle(QStyle* style)
{
    _pExpandMarkProgress = 0;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
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
            ElaControlStyle::drawInputFrame(painter, fopt->rect, 6, _themeMode, fopt->state, widget, _pExpandMarkProgress);
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
