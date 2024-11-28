#include "ElaKeyBinderPrivate.h"
#include "ElaKeyBinder.h"
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
    if (themeMode == ElaThemeType::Light)
    {
        palette.setColor(QPalette::WindowText, Qt::black);
        q->setPalette(palette);
    }
    else
    {
        palette.setColor(QPalette::WindowText, Qt::white);
        q->setPalette(palette);
    }
}
