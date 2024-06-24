#include "ElaTabBar.h"

#include "ElaTabBarStyle.h"
ElaTabBar::ElaTabBar(QWidget* parent)
    : QTabBar(parent)
{
    setObjectName("ElaTabBar");
    setStyleSheet("#ElaTabBar{background-color:transparent;}");
    setTabsClosable(true);
    setMovable(true);
    setStyle(new ElaTabBarStyle(style()));
}

ElaTabBar::~ElaTabBar()
{
}
