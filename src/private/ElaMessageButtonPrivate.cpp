#include "ElaMessageButtonPrivate.h"

#include <QPalette>

#include "ElaMessageButton.h"
ElaMessageButtonPrivate::ElaMessageButtonPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaMessageButtonPrivate::~ElaMessageButtonPrivate()
{
}

void ElaMessageButtonPrivate::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    Q_Q(ElaMessageButton);
    _themeMode = themeMode;
    QPalette palette;
    if (_themeMode == ElaApplicationType::Light)
    {
        palette.setColor(QPalette::ButtonText, Qt::black);
    }
    else
    {
        palette.setColor(QPalette::ButtonText, QColor(0xFE, 0xFE, 0xFE));
    }
    q->setPalette(palette);
}
