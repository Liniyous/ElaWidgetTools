#include "ElaTextPrivate.h"

#include "ElaText.h"
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
    if (q->isVisible())
    {
        _changeTheme();
    }
    else
    {
        QTimer::singleShot(1, this, [=] {
            _changeTheme();
        });
    }
}

void ElaTextPrivate::_changeTheme()
{
    Q_Q(ElaText);
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, _themeMode == ElaThemeType::Light ? Qt::black : Qt::white);
    q->setPalette(palette);
}
