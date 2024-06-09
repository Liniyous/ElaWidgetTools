#include "ElaToggleButtonPrivate.h"

#include "ElaApplication.h"
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
    _themeMode = ElaApplication::getInstance()->getThemeMode();
    q->setMouseTracking(true);
    q->setFixedSize(80, 38);
    q->setFont(QFont("Microsoft YaHei", 11));
    q->setObjectName("ElaToggleButton");
    q->setStyleSheet("#ElaToggleButton{background-color:transparent;}");
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode)
            { _themeMode = themeMode; });
}
