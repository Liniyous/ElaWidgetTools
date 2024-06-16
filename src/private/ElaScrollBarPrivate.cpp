#include "ElaScrollBarPrivate.h"

#include <QPropertyAnimation>

#include "ElaScrollBar.h"
#include "ElaScrollBarStyle.h"
ElaScrollBarPrivate::ElaScrollBarPrivate(QObject* parent)
    : QObject{parent}
{
}

void ElaScrollBarPrivate::_initStyle()
{
    Q_Q(ElaScrollBar);
    q->setSingleStep(1);
    q->setAttribute(Qt::WA_OpaquePaintEvent, false);
    _slideSmoothAnimation = new QPropertyAnimation(q, "value");
    _slideSmoothAnimation->setEasingCurve(QEasingCurve::OutSine);
    _slideSmoothAnimation->setDuration(300);
    connect(_slideSmoothAnimation, &QPropertyAnimation::finished, q, [=]() { _scrollValue = q->value(); });
    _rangeSmoothAnimation = new QPropertyAnimation(q, "maximum");
    _rangeSmoothAnimation->setEasingCurve(QEasingCurve::OutSine);
    _rangeSmoothAnimation->setDuration(300);
    connect(_rangeSmoothAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant& value) { q->update(); });
    connect(_rangeSmoothAnimation, &QPropertyAnimation::finished, q, [=]() { _isRangeAnimationFinished = true; });
    q->setStyle(new ElaScrollBarStyle(q->style()));
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
