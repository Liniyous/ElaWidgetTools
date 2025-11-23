#include "ElaCalendarPickerContainer.h"
#include <QPainter>

#include "ElaTheme.h"
ElaCalendarPickerContainer::ElaCalendarPickerContainer(QWidget* parent)
    : QWidget{parent}
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(0, 0, 0, 0);
    setObjectName("ElaCalendarPickerContainer");
    setStyleSheet("#ElaCalendarPickerContainer{background-color:transparent}");
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
}

ElaCalendarPickerContainer::~ElaCalendarPickerContainer()
{
}

void ElaCalendarPickerContainer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    eTheme->drawEffectShadow(&painter, rect(), 6, 5);
    painter.restore();
}
