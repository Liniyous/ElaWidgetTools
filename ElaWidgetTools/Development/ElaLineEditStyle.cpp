#include "ElaLineEditStyle.h"

#include "ElaControlStyle.h"
#include "ElaTheme.h"

#include <QPainter>
#include <QStyleOption>

ElaLineEditStyle::ElaLineEditStyle(QStyle* style)
{
    _pExpandMarkProgress = 0;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
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
            ElaControlStyle::drawInputFrame(painter, fopt->rect, widget ? qMax(0, widget->property("pBorderRadius").toInt()) : 6, _themeMode, fopt->state, widget, _pExpandMarkProgress);
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
