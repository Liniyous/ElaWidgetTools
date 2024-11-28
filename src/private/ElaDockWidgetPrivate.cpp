#include "ElaDockWidgetPrivate.h"

#include <QLinearGradient>

#include "ElaDockWidget.h"
#include "ElaDockWidgetTitleBar.h"
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
}
