#ifndef ELASCROLLAREA_H
#define ELASCROLLAREA_H

#include <QScrollArea>

#include "stdafx.h"
class ELA_EXPORT ElaScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit ElaScrollArea(QWidget* parent = nullptr);
    ~ElaScrollArea();

    void setIsGrabGesture(bool isEnable, qreal mousePressEventDelay = 0.5);
    void setIsGrabGesture(Qt::Orientation orientation, bool isEnable, qreal mousePressEventDelay = 0.5);
    bool getIsGrabGesture(Qt::Orientation orientation) const;

    void setIsAnimation(Qt::Orientation orientation, bool isAnimation);
    bool getIsAnimation(Qt::Orientation orientation) const;
};

#endif // ELASCROLLAREA_H
