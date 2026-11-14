#include "ElaCalendarPickerContainer.h"

#include "ElaCalendar.h"
#include "ElaTheme.h"

#include <QPainter>
#include <QPainterPath>
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
    // 阴影与日历实际的内缩边框共用轮廓 避免外层再出现一圈错位边缘
    QPainterPath path;
    const auto* calendar = findChild<ElaCalendar*>(QString(), Qt::FindDirectChildrenOnly);
    const QRectF body = QRectF(rect().marginsRemoved(eTheme->getShadowMargins())).adjusted(1, 1, -1, -1);
    const int radius = calendar ? calendar->getBorderRaiuds() : 8;
    path.addRoundedRect(body, radius, radius);
    eTheme->drawEffectShadow(&painter, path);
    painter.restore();
}
