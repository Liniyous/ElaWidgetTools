#include "ElaPromotionCardPrivate.h"

#include <QPoint>
#include <QPropertyAnimation>
#include <QtMath>

#include "ElaPromotionCard.h"
ElaPromotionCardPrivate::ElaPromotionCardPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaPromotionCardPrivate::~ElaPromotionCardPrivate()
{
}

qreal ElaPromotionCardPrivate::_getLongestDistance(QPoint point)
{
    Q_Q(ElaPromotionCard);
    qreal topLeftDis = _distance(point, QPoint(0, 0));
    qreal topRightDis = _distance(point, QPoint(q->width(), 0));
    qreal bottomLeftDis = _distance(point, QPoint(0, q->height()));
    qreal bottomRightDis = _distance(point, QPoint(q->width(), q->height()));
    QList<qreal> disList{topLeftDis, topRightDis, bottomLeftDis, bottomRightDis};
    std::sort(disList.begin(), disList.end());
    return disList[3];
}

qreal ElaPromotionCardPrivate::_distance(QPoint point1, QPoint point2)
{
    return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) + (point1.y() - point2.y()) * (point1.y() - point2.y()));
}

void ElaPromotionCardPrivate::_startHoverOpacityAnimation(bool isVisible)
{
    Q_Q(ElaPromotionCard);
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "pHoverOpacity");
    connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        q->update();
    });
    opacityAnimation->setDuration(250);
    opacityAnimation->setStartValue(_pHoverOpacity);
    opacityAnimation->setEndValue(isVisible ? 1 : 0);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
