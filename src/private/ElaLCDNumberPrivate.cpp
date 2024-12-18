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
    q->foregroundRole();
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, ElaThemeColor(themeMode, BasicText));
    q->setPalette(palette);
}
