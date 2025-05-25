#include "ElaStatusBar.h"

#include <QPainter>
#include <QTimer>

#include "ElaStatusBarStyle.h"
ElaStatusBar::ElaStatusBar(QWidget* parent)
    : QStatusBar(parent)
{
    setObjectName("ElaStatusBar");
    setStyleSheet("#ElaStatusBar{background-color:transparent;}");
    setFixedHeight(28);
    setContentsMargins(20, 0, 0, 0);
    setStyle(new ElaStatusBarStyle(style()));
}

ElaStatusBar::~ElaStatusBar()
{
}
