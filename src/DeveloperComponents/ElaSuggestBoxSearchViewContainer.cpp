#include "ElaSuggestBoxSearchViewContainer.h"

#include <QPainter>

#include "ElaTheme.h"
ElaSuggestBoxSearchViewContainer::ElaSuggestBoxSearchViewContainer(QWidget* parent)
    : QWidget{parent}
{
    setContentsMargins(8, 8, 8, 8);
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
    eTheme->drawEffectShadow(&painter, rect(), 6, 8);
    painter.setPen(ElaThemeColor(_themeMode, ComboBoxShapedFrameBorder));
    painter.setBrush(ElaThemeColor(_themeMode, SuggestBoxSearchViewContainerBase));
    QRect foregroundRect(6, 0, rect().width() - 2 * 6, rect().height() - 6);
    painter.drawRoundedRect(foregroundRect, 8, 8);
    painter.restore();
}
