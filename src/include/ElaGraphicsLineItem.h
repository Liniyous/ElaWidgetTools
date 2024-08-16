#ifndef ELAGRAPHICSLINEITEM_H
#define ELAGRAPHICSLINEITEM_H

#include <QGraphicsPathItem>

#include "stdafx.h"
class ElaGraphicsItem;
class ElaGraphicsLineItemPrivate;
class ELA_EXPORT ElaGraphicsLineItem : public QGraphicsPathItem
{
    Q_Q_CREATE(ElaGraphicsLineItem)
    Q_PRIVATE_CREATE_Q_H(QPointF, StartPoint);
    Q_PRIVATE_CREATE_Q_H(QPointF, EndPoint);
    Q_PRIVATE_CREATE_Q_H(ElaGraphicsItem*, StartItem);
    Q_PRIVATE_CREATE_Q_H(ElaGraphicsItem*, EndItem);
    Q_PRIVATE_CREATE_Q_H(int, StartItemPort);
    Q_PRIVATE_CREATE_Q_H(int, EndItemPort);

public:
    explicit ElaGraphicsLineItem(ElaGraphicsItem* startItem, ElaGraphicsItem* endItem, int startItemPort, int endItemPort, QGraphicsItem* parent = nullptr);
    explicit ElaGraphicsLineItem(QPointF startPoint, QPointF endPoint, QGraphicsItem* parent = nullptr);
    ~ElaGraphicsLineItem();

    bool isTargetLink(ElaGraphicsItem* item) const;
    bool isTargetLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2) const;
    bool isTargetLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2, int port1, int port2) const;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;
};

#endif // ELAGRAPHICSLINEITEM_H
