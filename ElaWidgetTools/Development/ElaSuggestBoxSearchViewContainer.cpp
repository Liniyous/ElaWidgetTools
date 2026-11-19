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
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
}

ElaSuggestBoxSearchViewContainer::~ElaSuggestBoxSearchViewContainer()
{
}

void ElaSuggestBoxSearchViewContainer::setBodyRect(const QRect& bodyRect)
{
    _bodyRect = bodyRect;
    update();
}

void ElaSuggestBoxSearchViewContainer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    const QMargins margins = eTheme->getShadowMargins();
    eTheme->drawEffectShadow(&painter, _bodyRect.marginsAdded(margins), margins, 8);
    painter.setPen(ElaThemeColor(_themeMode, BasicBorder));
    painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(_themeMode, PopupBase), "Background"));
    painter.drawRoundedRect(_bodyRect, 8, 8);
}
