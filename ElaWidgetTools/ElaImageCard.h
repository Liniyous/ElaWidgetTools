#ifndef ELAIMAGECARD_H
#define ELAIMAGECARD_H

#include <QWidget>

#include "ElaProperty.h"

class ElaImageCardPrivate;
class ELA_EXPORT ElaImageCard : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaImageCard)
    Q_PROPERTY_CREATE_Q_H(QImage, CardImage);
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(bool, IsPreserveAspectCrop)
    Q_PROPERTY_CREATE_Q_H(qreal, MaximumAspectRatio)
public:
    explicit ElaImageCard(QWidget* parent = nullptr);
    ~ElaImageCard();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAIMAGECARD_H
