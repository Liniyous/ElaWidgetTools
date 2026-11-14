#include "ElaSuggestBoxSearchViewContainer.h"

#include "ElaColorAnimation.h"
#include "ElaTheme.h"

#include <QPainter>

ElaSuggestBoxSearchViewContainer::ElaSuggestBoxSearchViewContainer(QWidget* parent)
    : QWidget{parent}
{
    setContentsMargins(eTheme->getShadowMargins() + QMargins(2, 8, 2, 2));
    setObjectName("ElaSuggestBoxSearchViewBaseWidget");
    setStyleSheet("#ElaSuggestBoxSearchViewBaseWidget{background-color:transparent}");
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaSuggestBoxSearchViewContainer::~ElaSuggestBoxSearchViewContainer()
{
}

void ElaSuggestBoxSearchViewContainer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    const QMargins margins = eTheme->getShadowMargins();
    eTheme->drawEffectShadow(&painter, rect(), margins, 8);
    painter.setPen(ElaThemeColor(_themeMode, BasicBorder));
    painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(_themeMode, PopupBase), "Background"));
    const QRect foregroundRect = rect().marginsRemoved(margins);
    painter.drawRoundedRect(foregroundRect, 8, 8);
    painter.restore();
}
