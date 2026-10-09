#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPROMOTIONVIEWPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPROMOTIONVIEWPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>
#include <QPixmap>

#include "ElaWidgetToolsDef.h"
class ElaPromotionCard;
class ElaPromotionView;
class ElaPromotionViewPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaPromotionView)
    ELA_PROPERTY_D(int, CardExpandWidth)
    ELA_PROPERTY_D(int, CardCollapseWidth)
    ELA_PROPERTY_D(int, CurrentIndex)
    ELA_PROPERTY_D(bool, IsAutoScroll)
    ELA_PROPERTY_D(int, AutoScrollInterval)
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

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPROMOTIONVIEWPRIVATE_H_

