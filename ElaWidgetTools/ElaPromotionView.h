#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROMOTIONVIEW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROMOTIONVIEW_H_

#include <QPixmap>
#include <QWidget>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaPromotionCard;
class ElaPromotionViewPrivate;
class ELA_EXPORT ElaPromotionView : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaPromotionView)
public:
    explicit ElaPromotionView(QWidget* parent = nullptr);
    ~ElaPromotionView();

    void setCardExpandWidth(int width);
    Q_REQUIRED_RESULT int getCardExpandWidth() const;
    void setCardCollapseWidth(int width);
    Q_REQUIRED_RESULT int getCardCollapseWidth() const;
    void setCurrentIndex(int index);
    Q_REQUIRED_RESULT int getCurrentIndex() const;
    void setIsAutoScroll(bool isAutoScroll);
    Q_REQUIRED_RESULT bool getIsAutoScroll() const;
    void setAutoScrollInterval(int autoScrollInterval);
    Q_REQUIRED_RESULT int getAutoScrollInterval() const;

    void appendPromotionCard(ElaPromotionCard* card);

protected:
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

Q_SIGNALS:
    Q_SIGNAL void pCardExpandWidthChanged();
    Q_SIGNAL void pCardCollapseWidthChanged();
    Q_SIGNAL void pCurrentIndexChanged();
    Q_SIGNAL void pIsAutoScrollChanged();
    Q_SIGNAL void pAutoScrollIntervalChanged();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROMOTIONVIEW_H_
