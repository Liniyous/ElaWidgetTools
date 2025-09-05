#include "ElaKeyBinderPrivate.h"
#include "ElaKeyBinder.h"
#include "ElaTheme.h"
#include <QTimer>
ElaKeyBinderPrivate::ElaKeyBinderPrivate(QObject* parent)
    : QObject(parent)
{
}

ElaKeyBinderPrivate::~ElaKeyBinderPrivate()
{
}

void ElaKeyBinderPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaKeyBinder);
    _themeMode = themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, ElaThemeColor(_themeMode, BasicText));
    q->setPalette(palette);
}
