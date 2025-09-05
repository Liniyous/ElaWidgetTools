#include "ElaTextPrivate.h"

#include "ElaText.h"
#include "ElaTheme.h"
#include <QTimer>
ElaTextPrivate::ElaTextPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaTextPrivate::~ElaTextPrivate()
{
}

void ElaTextPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaText);
    _themeMode = themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, ElaThemeColor(_themeMode, BasicText));
    q->setPalette(palette);
}
