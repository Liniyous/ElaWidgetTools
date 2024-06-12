#include "ElaPushButtonPrivate.h"

#include "ElaPushButton.h"

ElaPushButtonPrivate::ElaPushButtonPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaPushButtonPrivate::~ElaPushButtonPrivate()
{
}

void ElaPushButtonPrivate::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    Q_Q(ElaPushButton);
    _themeMode = themeMode;
    if (_themeMode == ElaApplicationType::Light)
    {
        QPalette palette = q->palette();
        palette.setColor(QPalette::ButtonText, _lightTextColor);
        q->setPalette(palette);
    }
    else
    {
        QPalette palette = q->palette();
        palette.setColor(QPalette::ButtonText, _darkTextColor);
        q->setPalette(palette);
    }
}
