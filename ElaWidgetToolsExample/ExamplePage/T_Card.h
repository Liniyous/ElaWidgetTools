#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_CARD_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_CARD_H_

#include "T_BasePage.h"

class ElaLCDNumber;
class ElaPromotionCard;
class ElaPromotionView;
class T_Card : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Card(QWidget* parent = nullptr);
    ~T_Card();

private:
    ElaLCDNumber* _lcdNumber{nullptr};
    ElaPromotionCard* _promotionCard{nullptr};
    ElaPromotionView* _promotionView{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_CARD_H_

