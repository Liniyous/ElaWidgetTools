#ifndef ELASCROLLAREA_H
#define ELASCROLLAREA_H

#include <QScrollArea>

#include "stdafx.h"

class ElaScrollAreaPrivate;
class ELA_EXPORT ElaScrollArea : public QScrollArea
{
    Q_OBJECT
    Q_Q_CREATE(ElaScrollArea)
public:
    explicit ElaScrollArea(QWidget* parent = nullptr);
    ~ElaScrollArea();

    void setIsGrabGesture(bool isEnable, qreal mousePressEventDelay = 0.5);

    void setIsOverShoot(Qt::Orientation orientation, bool isEnable);
    bool getIsOverShoot(Qt::Orientation orientation) const;

    void setIsAnimation(Qt::Orientation orientation, bool isAnimation);
    bool getIsAnimation(Qt::Orientation orientation) const;
};

#endif // ELASCROLLAREA_H
