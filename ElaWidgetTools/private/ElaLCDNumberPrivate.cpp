#include "ElaLCDNumberPrivate.h"
#include "ElaLCDNumber.h"
#include "ElaTheme.h"
#include <QTimer>
ElaLCDNumberPrivate::ElaLCDNumberPrivate(QObject* parent)
    : QObject(parent)
{
}

ElaLCDNumberPrivate::~ElaLCDNumberPrivate()
{
}

void ElaLCDNumberPrivate::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaLCDNumber);
    if (q->isVisible())
    {
        QPalette palette = q->palette();
        palette.setColor(QPalette::WindowText, ElaThemeColor(themeMode, BasicText));
        q->setPalette(palette);
    }
    else
    {
        QTimer::singleShot(1, this, [=] {
            QPalette palette = q->palette();
            palette.setColor(QPalette::WindowText, ElaThemeColor(themeMode, BasicText));
            q->setPalette(palette);
        });
    }
}
