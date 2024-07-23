#include "ElaWidgetPrivate.h"

#include "ElaTheme.h"
#include "ElaWidget.h"

ElaWidgetPrivate::ElaWidgetPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaWidgetPrivate::~ElaWidgetPrivate()
{
}

void ElaWidgetPrivate::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    _windowLinearGradient->setColorAt(0, ElaThemeColor(themeMode, WindowBaseStart));
    _windowLinearGradient->setColorAt(1, ElaThemeColor(themeMode, WindowBaseEnd));
}
