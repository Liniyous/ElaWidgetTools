#include "ElaToggleButtonPrivate.h"

#include "ElaTheme.h"
#include "ElaToggleButton.h"
ElaToggleButtonPrivate::ElaToggleButtonPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaToggleButtonPrivate::~ElaToggleButtonPrivate()
{
}

void ElaToggleButtonPrivate::_initStyle()
{
    Q_Q(ElaToggleButton);
    _pBorderRadius = 3;
    _themeMode = eTheme->getThemeMode();
    q->setMouseTracking(true);
    q->setFixedSize(80, 38);
    QFont font = q->font();
    font.setPointSize(11);
    q->setFont(font);
    q->setObjectName("ElaToggleButton");
    q->setStyleSheet("#ElaToggleButton{background-color:transparent;}");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}
