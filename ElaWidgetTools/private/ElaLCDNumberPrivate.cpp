#include "ElaLCDNumberPrivate.h"
#include "ElaLCDNumber.h"
#include "ElaTheme.h"
ElaLCDNumberPrivate::ElaLCDNumberPrivate(QObject* parent)
    : QObject(parent)
{
}

ElaLCDNumberPrivate::~ElaLCDNumberPrivate()
{
}

void ElaLCDNumberPrivate::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaLCDNumber);
    _themeMode = themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, ElaThemeColor(themeMode, BasicText));
    q->setPalette(palette);
}
