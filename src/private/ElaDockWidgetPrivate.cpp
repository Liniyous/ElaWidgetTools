#include "ElaDockWidgetPrivate.h"

#include <QLinearGradient>

#include "ElaDockWidget.h"
ElaDockWidgetPrivate::ElaDockWidgetPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaDockWidgetPrivate::~ElaDockWidgetPrivate()
{
}

void ElaDockWidgetPrivate::onThemeModeChanged(ElaApplicationType::ThemeMode themeMode)
{
    Q_Q(ElaDockWidget);
    q->update();
    _themeMode = themeMode;
    if (themeMode == ElaApplicationType::Light)
    {
        _windowLinearGradient->setColorAt(0, QColor(0xF3, 0xF2, 0xF9));
        _windowLinearGradient->setColorAt(1, QColor(0xF4, 0xF1, 0xF8));
    }
    else
    {
        _windowLinearGradient->setColorAt(0, QColor(0x1A, 0x1A, 0x1A));
        _windowLinearGradient->setColorAt(1, QColor(0x1A, 0x1A, 0x1A));
    }
}
