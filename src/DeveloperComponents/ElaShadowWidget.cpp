#include "ElaShadowWidget.h"

#include <QGraphicsOpacityEffect>
ElaShadowWidget::ElaShadowWidget(QWidget* parent)
    : QWidget{parent}
{
    setObjectName("ElaShadowWidget");
    setStyleSheet("#ElaShadowWidget{background-color:transparent}");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(0xDA, 0xDA, 0xDA));
    shadow->setBlurRadius(50);
    setGraphicsEffect(shadow);
}

ElaShadowWidget::~ElaShadowWidget()
{
}
