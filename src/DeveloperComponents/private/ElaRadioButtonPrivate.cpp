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
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, ElaThemeColor(themeMode, WindowText));
    q->setPalette(palette);
}
