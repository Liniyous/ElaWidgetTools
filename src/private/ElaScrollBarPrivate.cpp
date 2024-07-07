#include "ElaScrollBarPrivate.h"

#include <QPropertyAnimation>

#include "ElaScrollBar.h"
ElaScrollBarPrivate::ElaScrollBarPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaScrollBarPrivate::~ElaScrollBarPrivate()
{
}

void ElaScrollBarPrivate::onRangeChanged(int min, int max)
{
    Q_Q(ElaScrollBar);
    if (q->isVisible() && _pisAnimation)
    {
        QPropertyAnimation* rangeSmoothAnimation = new QPropertyAnimation(this, "pTargetMaximum");
        connect(rangeSmoothAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            q->blockSignals(true);
            q->setMaximum(value.toUInt());
            q->blockSignals(false);
            q->update();
        });
        rangeSmoothAnimation->setEasingCurve(QEasingCurve::OutSine);
        rangeSmoothAnimation->setDuration(250);
        rangeSmoothAnimation->setStartValue(_pTargetMaximum);
        rangeSmoothAnimation->setEndValue(max);
        rangeSmoothAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        _pTargetMaximum = max;
    }
}

void ElaScrollBarPrivate::_scroll(int value)
{
    Q_Q(ElaScrollBar);
    _scrollValue -= value;
    _slideSmoothAnimation->stop();
    _slideSmoothAnimation->setStartValue(q->value());
    _slideSmoothAnimation->setEndValue(_scrollValue);
    _slideSmoothAnimation->start();
}
