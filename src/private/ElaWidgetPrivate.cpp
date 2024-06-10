#include "ElaWidgetPrivate.h"

#include "ElaApplication.h"
#include "ElaWidget.h"

ElaWidgetPrivate::ElaWidgetPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaWidgetPrivate::~ElaWidgetPrivate()
{
}

void ElaWidgetPrivate::onThemeModeChanged(ElaApplicationType::ThemeMode themeMode)
{
    if (ElaApplication::getInstance()->getThemeMode() == ElaApplicationType::Light)
    {
        _windowLinearGradient->setColorAt(0, QColor(0xF2, 0xF2, 0xF9));
        _windowLinearGradient->setColorAt(1, QColor(0xF9, 0xEF, 0xF6));
    }
    else
    {
        _windowLinearGradient->setColorAt(0, QColor(0x1A, 0x1A, 0x1A));
        _windowLinearGradient->setColorAt(1, QColor(0x1A, 0x1A, 0x1A));
    }
}
