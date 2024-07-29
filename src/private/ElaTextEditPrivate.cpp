#include "ElaTextEditPrivate.h"

#include "ElaTextEdit.h"

ElaTextEditPrivate::ElaTextEditPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaTextEditPrivate::~ElaTextEditPrivate()
{
}

void ElaTextEditPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaTextEdit);
    _themeMode = themeMode;
    QPalette palette = q->palette();
    if (themeMode == ElaThemeType::Light)
    {
        palette.setColor(QPalette::Text, Qt::black);
        q->setPalette(palette);
    }
    else
    {
        palette.setColor(QPalette::Text, Qt::white);
        q->setPalette(palette);
    }
}