#include "ElaGraphicsLineItem.h"

#include <QGraphicsScene>
#include <QPainter>

#include "ElaGraphicsItem.h"
#include "ElaGraphicsLineItemPrivate.h"
Q_PRIVATE_CREATE_Q_CPP(ElaGraphicsLineItem, QPointF, StartPoint);
Q_PRIVATE_CREATE_Q_CPP(ElaGraphicsLineItem, QPointF, EndPoint);
Q_PRIVATE_CREATE_Q_CPP(ElaGraphicsLineItem, ElaGraphicsItem*, StartItem);
Q_PRIVATE_CREATE_Q_CPP(ElaGraphicsLineItem, ElaGraphicsItem*, EndItem);
Q_PRIVATE_CREATE_Q_CPP(ElaGraphicsLineItem, int, StartItemPort);
Q_PRIVATE_CREATE_Q_CPP(ElaGraphicsLineItem, int, EndItemPort);
ElaGraphicsLineItem::ElaGraphicsLineItem(ElaGraphicsItem* startItem, ElaGraphicsItem* endItem, int startItemPort, int endItemPort, QGraphicsItem* parent)
    : QGraphicsPathItem(parent), d_ptr(new ElaGraphicsLineItemPrivate())
{
    Q_D(ElaGraphicsLineItem);
    d->q_ptr = this;
    d->_pStartItem = startItem;
    d->_pEndItem = endItem;
    d->_pStartItemPort = startItemPort;
    d->_pEndItemPort = endItemPort;
    d->_linkItemMap.insert(d->_pStartItem, d->_pStartItemPort);
    d->_linkItemMap.insert(d->_pEndItem, d->_pEndItemPort);
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | ItemAcceptsInputMethod);
}

ElaGraphicsLineItem::ElaGraphicsLineItem(QPointF startPoint, QPointF endPoint, QGraphicsItem* parent)
    : QGraphicsPathItem(parent), d_ptr(new ElaGraphicsLineItemPrivate())
{
    Q_D(ElaGraphicsLineItem);
    d->q_ptr = this;
    d->_pStartPoint = startPoint;
    d->_pEndPoint = endPoint;
    d->_isCreateWithItem = false;
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | ItemAcceptsInputMethod);
}

ElaGraphicsLineItem::~ElaGraphicsLineItem()
{
}

bool ElaGraphicsLineItem::isTargetLink(ElaGraphicsItem* item) const
{
    Q_D(const ElaGraphicsLineItem);
    if (d->_linkItemMap.contains(item))
    {
        return true;
    }
    return false;
}

bool ElaGraphicsLineItem::isTargetLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2) const
{
    Q_D(const ElaGraphicsLineItem);
    if (d->_linkItemMap.contains(item1) && d->_linkItemMap.contains(item2))
    {
        return true;
    }
    return false;
}

bool ElaGraphicsLineItem::isTargetLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2, int port1, int port2) const
{
    Q_D(const ElaGraphicsLineItem);
    if (d->_linkItemMap.value(item1) == port1 && d->_linkItemMap.value(item2) == port2)
    {
        return true;
    }
    return false;
}

void ElaGraphicsLineItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_D(ElaGraphicsLineItem);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::black, 3));
    QPainterPath path;
    qreal pathXStart = 0;
    qreal pathYStart = 0;
    qreal pathXEnd = 0;
    qreal pathYEnd = 0;
    if (d->_isCreateWithItem)
    {
        pathXStart = d->_pStartItem->x();
        pathYStart = d->_pStartItem->y();
        pathXEnd = d->_pEndItem->x();
        pathYEnd = d->_pEndItem->y();
    }
    else
    {
        pathXStart = d->_pStartPoint.x();
        pathYStart = d->_pStartPoint.y();
        pathXEnd = d->_pEndPoint.x();
        pathYEnd = d->_pEndPoint.y();
    }
    path.moveTo(pathXStart, pathYStart); // 设置起始点
    path.cubicTo((pathXStart + pathXEnd) / 2, pathYStart, (pathXStart + pathXEnd) / 2, pathYEnd, pathXEnd, pathYEnd);
    setPath(path);
    painter->drawPath(path);
    painter->restore();
}

QRectF ElaGraphicsLineItem::boundingRect() const
{
    if (!QGraphicsPathItem::boundingRect().isValid())
    {
        return scene()->sceneRect();
    }
    return QGraphicsPathItem::boundingRect();
}
