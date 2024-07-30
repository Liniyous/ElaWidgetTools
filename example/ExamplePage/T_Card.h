#ifndef T_CARD_H
#define T_CARD_H

#include <ElaScrollPage.h>

class ElaPromotionCard;
class ElaPromotionView;
class T_Card : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit T_Card(QWidget* parent = nullptr);
    ~T_Card();

private:
    ElaPromotionCard* _promotionCard{nullptr};
    ElaPromotionView* _promotionView{nullptr};
};

#endif // T_CARD_H
