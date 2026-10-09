#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROMOTIONCARD_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROMOTIONCARD_H_
#include <QWidget>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaPromotionCardPrivate;
class ELA_EXPORT ElaPromotionCard : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaPromotionCard)
    ELA_PROPERTY_Q_H(int, BorderRadius)
    ELA_PROPERTY_Q_H(QPixmap, CardPixmap)
    ELA_PROPERTY_Q_H(QString, CardTitle)
    ELA_PROPERTY_Q_H(QString, PromotionTitle)
    ELA_PROPERTY_Q_H(QString, Title)
    ELA_PROPERTY_Q_H(QString, SubTitle)
    ELA_PROPERTY_Q_H(QColor, CardTitleColor)
    ELA_PROPERTY_Q_H(QColor, PromotionTitleColor)
    ELA_PROPERTY_Q_H(QColor, PromotionTitleBaseColor)
    ELA_PROPERTY_Q_H(QColor, TitleColor)
    ELA_PROPERTY_Q_H(QColor, SubTitleColor)
    ELA_PROPERTY_Q_H(int, CardTitlePixelSize)
    ELA_PROPERTY_Q_H(int, PromotionTitlePixelSize)
    ELA_PROPERTY_Q_H(int, TitlePixelSize)
    ELA_PROPERTY_Q_H(int, SubTitlePixelSize)
    Q_PROPERTY(qreal pHorizontalCardPixmapRatio READ getHorizontalCardPixmapRatio WRITE setHorizontalCardPixmapRatio NOTIFY pHorizontalCardPixmapRatioChanged)
public:
    explicit ElaPromotionCard(QWidget* parent = nullptr);
    ~ElaPromotionCard();

    void setHorizontalCardPixmapRatio(qreal pixmapRatio);
    Q_REQUIRED_RESULT qreal getHorizontalCardPixmapRatio() const;
    void setVerticalCardPixmapRatio(qreal pixmapRatio);
    Q_REQUIRED_RESULT qreal getVerticalCardPixmapRatio() const;

Q_SIGNALS:
    Q_SIGNAL void pHorizontalCardPixmapRatioChanged();
    Q_SIGNAL void pVerticalCardPixmapRatioChanged();
    Q_SIGNAL void promotionCardClicked();

protected:
    virtual bool event(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROMOTIONCARD_H_
