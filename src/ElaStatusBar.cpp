#include "ElaStatusBar.h"

#include <QPainter>
#include <QTimer>

#include "ElaStatusBarStyle.h"
#include "ElaTheme.h"

ElaStatusBar::ElaStatusBar(QWidget* parent)
    : QStatusBar(parent)
{
    setFixedHeight(28);
    setContentsMargins(20, 0, 0, 0);
    setStyle(new ElaStatusBarStyle(style()));
}

ElaStatusBar::~ElaStatusBar()
{
}

void ElaStatusBar::paintEvent(QPaintEvent *event) {
    QStatusBar::paintEvent(event);

    QPainter painter(this);
    QPen pen;
    pen.setWidth(4);
    painter.setPen(pen);
    if(eTheme->getThemeMode() == ElaThemeType::Light)
        painter.setPen(QColor::fromString("#e0e1dd"));
    else painter.setPen(QColor::fromString("#343a40"));
    painter.drawLine(0,0,this->width(),0);
}
