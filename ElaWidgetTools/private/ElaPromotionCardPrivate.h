#ifndef ELAPROMOTIONCARDPRIVATE_H
#define ELAPROMOTIONCARDPRIVATE_H

#include <QObject>
#include <QPixmap>
#include <QPoint>

#include "ElaProperty.h"
class QRadialGradient;
class ElaPromotionCard;
class ElaPromotionCardPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaPromotionCard)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QPixmap, CardPixmap)
    Q_PROPERTY_CREATE_D(QString, CardTitle)
    Q_PROPERTY_CREATE_D(QString, PromotionTitle)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, SubTitle)
    Q_PROPERTY_CREATE_D(QColor, CardTitleColor)
    Q_PROPERTY_CREATE_D(QColor, PromotionTitleColor)
    Q_PROPERTY_CREATE_D(QColor, PromotionTitleBaseColor)
    Q_PROPERTY_CREATE_D(QColor, TitleColor)
    Q_PROPERTY_CREATE_D(QColor, SubTitleColor)
    Q_PROPERTY_CREATE_D(int, CardTitlePixelSize)
    Q_PROPERTY_CREATE_D(int, PromotionTitlePixelSize)
    Q_PROPERTY_CREATE_D(int, TitlePixelSize)
    Q_PROPERTY_CREATE_D(int, SubTitlePixelSize)
    Q_PROPERTY_CREATE_D(qreal, HorizontalCardPixmapRatio)
    Q_PROPERTY_CREATE_D(qreal, VerticalCardPixmapRatio)
    Q_PROPERTY_CREATE(qreal, PressRadius)
    Q_PROPERTY_CREATE(qreal, HoverOpacity)
    Q_PROPERTY_CREATE(qreal, PressOpacity)
public:
    explicit ElaPromotionCardPrivate(QObject* parent = nullptr);
    ~ElaPromotionCardPrivate();

private:
    int _shadowBorderWidth{6};
    QRadialGradient* _hoverGradient{nullptr};
    QRadialGradient* _pressGradient{nullptr};
    bool _isPressAnimationFinished{true};
    qreal _getLongestDistance(QPoint point);
    qreal _distance(QPoint point1, QPoint point2);
    void _startHoverOpacityAnimation(bool isVisible);
};

#endif // ELAPROMOTIONCARDPRIVATE_H
