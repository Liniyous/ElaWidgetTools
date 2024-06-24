#include "ElaTabWidget.h"

#include <QPainter>

#include "ElaTabBar.h"
ElaTabWidget::ElaTabWidget(QWidget* parent)
    : QTabWidget(parent)
{
    _pIsTabTransparent = false;
    setObjectName("ElaTabWidget");
    setStyleSheet("#ElaTabWidget{background-color:transparent;}");
    setTabBar(new ElaTabBar(this));
}

ElaTabWidget::~ElaTabWidget()
{
}

void ElaTabWidget::setTabPosition(TabPosition position)
{
    if (position == QTabWidget::North || position == QTabWidget::South)
    {
        QTabWidget::setTabPosition(position);
    }
}

void ElaTabWidget::paintEvent(QPaintEvent* event)
{
    if (!_pIsTabTransparent)
    {
        QTabWidget::paintEvent(event);
    }
}
