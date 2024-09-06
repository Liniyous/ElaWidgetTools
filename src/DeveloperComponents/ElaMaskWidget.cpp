#include "ElaMaskWidget.h"

#include <QPainter>
#include <QPropertyAnimation>
ElaMaskWidget::ElaMaskWidget(QWidget* parent)
    : QWidget{parent}
{
    setObjectName("ElaMaskWidget");
    setStyleSheet("#ElaMaskWidget{background-color:transparent;}");
    _pMaskAlpha = 0;
}

ElaMaskWidget::~ElaMaskWidget()
{
}

void ElaMaskWidget::doMaskAnimation(int endValue)
{
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "pMaskAlpha");
    connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() {
        if (endValue == 0)
        {
            setVisible(false);
        }
    });
    opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    opacityAnimation->setDuration(250);
    opacityAnimation->setStartValue(_pMaskAlpha);
    opacityAnimation->setEndValue(endValue);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaMaskWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0x00, 0x00, 0x00, _pMaskAlpha));
    painter.drawRect(rect());
    painter.restore();
}
