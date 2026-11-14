#include "ElaStatusBar.h"

#include "ElaStatusBarStyle.h"

#include <QPainter>
#include <QTimer>

ElaStatusBar::ElaStatusBar(QWidget* parent)
    : QStatusBar(parent)
{
    setObjectName("ElaStatusBar");
    setStyleSheet("#ElaStatusBar{background-color:transparent;}");
    setFixedHeight(28);
    setContentsMargins(20, 0, 0, 0);
    auto* statusBarStyle = new ElaStatusBarStyle(style());
    statusBarStyle->setParent(this);
    setStyle(statusBarStyle);
}

ElaStatusBar::~ElaStatusBar() = default;
