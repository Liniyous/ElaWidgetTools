#include "ElaTextPrivate.h"

#include "ElaText.h"

ElaTextPrivate::ElaTextPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaTextPrivate::~ElaTextPrivate()
{
}

void ElaTextPrivate::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    Q_Q(ElaText);
    QPalette palette = q->palette();
    if (themeMode == ElaApplicationType::Light)
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
