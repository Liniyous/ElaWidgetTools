#include "ElaRadioButtonPrivate.h"

#include "ElaRadioButton.h"
#include "ElaTheme.h"
ElaRadioButtonPrivate::ElaRadioButtonPrivate(QObject* parent)
    : QObject(parent)
{
}

ElaRadioButtonPrivate::~ElaRadioButtonPrivate()
{
}

void ElaRadioButtonPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaRadioButton);
    _themeMode = themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, ElaThemeColor(themeMode, BasicText));
    q->setPalette(palette);
}
