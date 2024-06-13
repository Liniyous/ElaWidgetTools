#ifndef ELAIMAGECARD_H
#define ELAIMAGECARD_H

#include <QWidget>

#include "stdafx.h"
class ELA_EXPORT ElaImageCard : public QWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(QImage, CardImage);
    Q_PROPERTY_CREATE(int, BorderRadius)
    Q_PROPERTY_CREATE(bool, IsPreserveAspectCrop)
    Q_PROPERTY_CREATE(qreal, MaximumAspectRatio)
public:
    explicit ElaImageCard(QWidget* parent = nullptr);
    ~ElaImageCard();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAIMAGECARD_H
