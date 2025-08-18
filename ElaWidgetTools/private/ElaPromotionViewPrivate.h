#ifndef ELAPROMOTIONVIEWPRIVATE_H
#define ELAPROMOTIONVIEWPRIVATE_H

#include <QObject>
#include <QPixmap>

#include "ElaDef.h"
class ElaPromotionCard;
class ElaPromotionView;
class ElaPromotionViewPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaPromotionView)
    Q_PROPERTY_CREATE_D(int, CardExpandWidth)
    Q_PROPERTY_CREATE_D(int, CardCollapseWidth)
    Q_PROPERTY_CREATE_D(int, CurrentIndex)
    Q_PROPERTY_CREATE_D(bool, IsAutoScroll)
    Q_PROPERTY_CREATE_D(int, AutoScrollInterval)
public:
    explicit ElaPromotionViewPrivate(QObject* parent = nullptr);
    ~ElaPromotionViewPrivate();
    void onPromotionCardClicked(ElaPromotionCard* clickedCard);

private:
    ElaThemeType::ThemeMode _themeMode;
    QTimer* _autoScrollTimer{nullptr};
    int _leftPadding{40};
    int _cardSpacing{5};
    int _bottomMargin{30};
    int _indicatorSpacing{12};
    bool _isAllowSwitch{true};
    QList<ElaPromotionCard*> _promotionCardList;
    void _startCardGeometryAnimation(ElaPromotionCard* card, QRect start, QRect end);
    void _startHorizontalCardPixmapRatioAnimation(ElaPromotionCard* card, qreal start, qreal end);
    void _updatePromotionCardGeometry();
    int _getAdjacentIndex(Qt::LayoutDirection direction, int index);
    int _getRightLimitX();
};

#endif // ELAPROMOTIONVIEWPRIVATE_H
