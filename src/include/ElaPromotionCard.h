#ifndef ELAPROMOTIONCARD_H
#define ELAPROMOTIONCARD_H
#include <QWidget>

#include "stdafx.h"
class ElaPromotionCardPrivate;
class ELA_EXPORT ElaPromotionCard : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaPromotionCard)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QPixmap, CardPixmap)
    Q_PROPERTY_CREATE_Q_H(QString, CardTitle)
    Q_PROPERTY_CREATE_Q_H(QString, PromotionTitle)
    Q_PROPERTY_CREATE_Q_H(QString, Title)
    Q_PROPERTY_CREATE_Q_H(QString, SubTitle)
    Q_PROPERTY_CREATE_Q_H(QColor, CardTitleColor)
    Q_PROPERTY_CREATE_Q_H(QColor, PromotionTitleColor)
    Q_PROPERTY_CREATE_Q_H(QColor, PromotionTitleBaseColor)
    Q_PROPERTY_CREATE_Q_H(QColor, TitleColor)
    Q_PROPERTY_CREATE_Q_H(QColor, SubTitleColor)
    Q_PROPERTY_CREATE_Q_H(int, CardTitlePixelSize)
    Q_PROPERTY_CREATE_Q_H(int, PromotionTitlePixelSize)
    Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize)
    Q_PROPERTY_CREATE_Q_H(int, SubTitlePixelSize)
    Q_PROPERTY_CREATE_Q_H(qreal, HorizontalCardPixmapRatio)
    Q_PROPERTY_CREATE_Q_H(qreal, VerticalCardPixmapRatio)
public:
    explicit ElaPromotionCard(QWidget* parent = nullptr);
    ~ElaPromotionCard();
Q_SIGNALS:
    Q_SIGNAL void promotionCardClicked();

protected:
    virtual bool event(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAPROMOTIONCARD_H
