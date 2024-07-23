#include "ElaDockWidgetPrivate.h"

#include <QLinearGradient>

#include "ElaDockWidget.h"
#include "ElaTheme.h"
ElaDockWidgetPrivate::ElaDockWidgetPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaDockWidgetPrivate::~ElaDockWidgetPrivate()
{
}

void ElaDockWidgetPrivate::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaDockWidget);
    q->update();
    _themeMode = themeMode;
    _windowLinearGradient->setColorAt(0, ElaThemeColor(themeMode, DockWidgetBaseStart));
    _windowLinearGradient->setColorAt(1, ElaThemeColor(themeMode, DockWidgetBaseEnd));
}
