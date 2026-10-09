#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPROMOTIONCARDPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPROMOTIONCARDPRIVATE_H_

#include <QObject>
#include <QPixmap>
#include <QPoint>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class QRadialGradient;
class ElaPromotionCard;
class ElaPromotionCardPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaPromotionCard)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY_D(QPixmap, CardPixmap)
    ELA_PROPERTY_D(QString, CardTitle)
    ELA_PROPERTY_D(QString, PromotionTitle)
    ELA_PROPERTY_D(QString, Title)
    ELA_PROPERTY_D(QString, SubTitle)
    ELA_PROPERTY_D(QColor, CardTitleColor)
    ELA_PROPERTY_D(QColor, PromotionTitleColor)
    ELA_PROPERTY_D(QColor, PromotionTitleBaseColor)
    ELA_PROPERTY_D(QColor, TitleColor)
    ELA_PROPERTY_D(QColor, SubTitleColor)
    ELA_PROPERTY_D(int, CardTitlePixelSize)
    ELA_PROPERTY_D(int, PromotionTitlePixelSize)
    ELA_PROPERTY_D(int, TitlePixelSize)
    ELA_PROPERTY_D(int, SubTitlePixelSize)
    ELA_PROPERTY_D(qreal, HorizontalCardPixmapRatio)
    ELA_PROPERTY_D(qreal, VerticalCardPixmapRatio)
    ELA_PROPERTY(qreal, PressRadius)
    ELA_PROPERTY(qreal, HoverOpacity)
    ELA_PROPERTY(qreal, PressOpacity)
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

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPROMOTIONCARDPRIVATE_H_

