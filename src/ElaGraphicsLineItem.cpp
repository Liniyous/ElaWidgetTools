#include "ElaGraphicsLineItem.h"

#include <QGraphicsScene>
#include <QPainter>

#include "ElaGraphicsItem.h"
ElaGraphicsLineItem::ElaGraphicsLineItem(ElaGraphicsItem* startItem, ElaGraphicsItem* endItem, int startItemPort, int endItemPort, QGraphicsItem* parent)
    : QGraphicsPathItem(parent)
{
    _pStartItem = startItem;
    _pEndItem = endItem;
    _pStartItemPort = startItemPort;
    _pEndItemPort = endItemPort;
    _linkItemMap.insert(_pStartItem, _pStartItemPort);
    _linkItemMap.insert(_pEndItem, _pEndItemPort);
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | ItemAcceptsInputMethod);
}

ElaGraphicsLineItem::ElaGraphicsLineItem(QPointF startPoint, QPointF endPoint, QGraphicsItem* parent)
{
    _pStartPoint = startPoint;
    _pEndPoint = endPoint;
    _isCreateWithItem = false;
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | ItemAcceptsInputMethod);
}

ElaGraphicsLineItem::~ElaGraphicsLineItem()
{
}

bool ElaGraphicsLineItem::isTargetLink(ElaGraphicsItem* item)
{
    if (_linkItemMap.contains(item))
    {
        return true;
    }
    return false;
}

bool ElaGraphicsLineItem::isTargetLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2)
{
    if (_linkItemMap.contains(item1) && _linkItemMap.contains(item2))
    {
        return true;
    }
    return false;
}

bool ElaGraphicsLineItem::isTargetLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2, int port1, int port2)
{
    if (_linkItemMap.value(item1) == port1 && _linkItemMap.value(item2) == port2)
    {
        return true;
    }
    return false;
}

void ElaGraphicsLineItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::black, 3));
    QPainterPath path;
    qreal pathXStart = 0;
    qreal pathYStart = 0;
    qreal pathXEnd = 0;
    qreal pathYEnd = 0;
    if (_isCreateWithItem)
    {
        pathXStart = _pStartItem->x();
        pathYStart = _pStartItem->y();
        pathXEnd = _pEndItem->x();
        pathYEnd = _pEndItem->y();
    }
    else
    {
        pathXStart = _pStartPoint.x();
        pathYStart = _pStartPoint.y();
        pathXEnd = _pEndPoint.x();
        pathYEnd = _pEndPoint.y();
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
