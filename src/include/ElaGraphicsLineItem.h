#ifndef ELAGRAPHICSLINEITEM_H
#define ELAGRAPHICSLINEITEM_H

#include <QGraphicsPathItem>

#include "stdafx.h"
class ElaGraphicsItem;
class ELA_EXPORT ElaGraphicsLineItem : public QGraphicsPathItem
{
    Q_PRIVATE_CREATE(QPointF, StartPoint);
    Q_PRIVATE_CREATE(QPointF, EndPoint);
    Q_PRIVATE_CREATE(ElaGraphicsItem*, StartItem);
    Q_PRIVATE_CREATE(ElaGraphicsItem*, EndItem);
    Q_PRIVATE_CREATE(int, StartItemPort);
    Q_PRIVATE_CREATE(int, EndItemPort);

public:
    explicit ElaGraphicsLineItem(ElaGraphicsItem* startItem, ElaGraphicsItem* endItem, int startItemPort, int endItemPort, QGraphicsItem* parent = nullptr);
    explicit ElaGraphicsLineItem(QPointF startPoint, QPointF endPoint, QGraphicsItem* parent = nullptr);
    ~ElaGraphicsLineItem();
    bool isTargetLink(ElaGraphicsItem* item);
    bool isTargetLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2);
    bool isTargetLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2, int port1, int port2);

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;

private:
    QMap<ElaGraphicsItem*, int> _linkItemMap;
    bool _isCreateWithItem{true};
};

#endif // ELAGRAPHICSLINEITEM_H
