#include "ElaGraphicsItem.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>
#include <QUuid>

#include "ElaGraphicsScene.h"
#include "private/ElaGraphicsItemPrivate.h"
#include "private/ElaGraphicsScenePrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsItem, int, Width)
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsItem, int, Height)
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsItem, QImage, ItemImage)
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsItem, QImage, ItemSelectedImage)
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsItem, QString, ItemName)
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsItem, QVariantMap, DataRoutes)
ElaGraphicsItem::ElaGraphicsItem(QGraphicsItem* parent)
    : QGraphicsObject(parent), d_ptr(new ElaGraphicsItemPrivate())
{
    Q_D(ElaGraphicsItem);
    d->q_ptr = this;
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | ItemAcceptsInputMethod);
    d->_pWidth = 50;
    d->_pHeight = 50;
    d->_itemUID = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    d->_pItemImage = QImage(":/include/Image/Moon.jpg");
    d->_pItemSelectedImage = QImage(":/include/Image/Cirno.jpg");
    d->_pItemName = "";
    d->_pMaxLinkPortCount = 1;
    d->_currentLinkPortState.resize(1);
    d->_currentLinkPortState.fill(false);
}

ElaGraphicsItem::ElaGraphicsItem(int width, int height, QGraphicsItem* parent)
    : ElaGraphicsItem(parent)
{
    Q_D(ElaGraphicsItem);
    d->_pWidth = width;
    d->_pHeight = height;
}

ElaGraphicsItem::~ElaGraphicsItem()
{
}

void ElaGraphicsItem::setMaxLinkPortCount(int maxLinkPortCount)
{
    Q_D(ElaGraphicsItem);
    if (maxLinkPortCount < 0)
    {
        maxLinkPortCount = 0;
    }
    d->_pMaxLinkPortCount = maxLinkPortCount;
    if (d->_currentLinkPortState.count() > maxLinkPortCount)
    {
        while (d->_currentLinkPortState.count() > maxLinkPortCount)
        {
            d->_currentLinkPortState.removeLast();
        }
    }
    else
    {
        while (d->_currentLinkPortState.count() < maxLinkPortCount)
        {
            d->_currentLinkPortState.append(false);
        }
    }
}

int ElaGraphicsItem::getMaxLinkPortCount() const
{
    Q_D(const ElaGraphicsItem);
    return d->_pMaxLinkPortCount;
}

QString ElaGraphicsItem::getItemUID() const
{
    return d_ptr->_itemUID;
}

void ElaGraphicsItem::setLinkPortState(bool isFullLink)
{
    Q_D(ElaGraphicsItem);
    d->_currentLinkPortState.fill(isFullLink);
}

void ElaGraphicsItem::setLinkPortState(bool isLink, int portIndex)
{
    Q_D(ElaGraphicsItem);
    if (portIndex >= 0 && portIndex < d->_pMaxLinkPortCount)
    {
        d->_currentLinkPortState[portIndex] = isLink;
    }
}

QVector<bool> ElaGraphicsItem::getLinkPortState() const
{
    Q_D(const ElaGraphicsItem);
    return d->_currentLinkPortState;
}

bool ElaGraphicsItem::getLinkPortState(int portIndex) const
{
    Q_D(const ElaGraphicsItem);
    if (portIndex >= 0 && portIndex < d->_pMaxLinkPortCount)
    {
        return d->_currentLinkPortState[portIndex];
    }
    return false;
}

int ElaGraphicsItem::getUsedLinkPortCount() const
{
    Q_D(const ElaGraphicsItem);
    int currentLinkPortCount = 0;
    for (auto isLink : d->_currentLinkPortState)
    {
        if (isLink)
        {
            currentLinkPortCount++;
        }
    }
    return currentLinkPortCount;
}

QVector<int> ElaGraphicsItem::getUsedLinkPort() const
{
    Q_D(const ElaGraphicsItem);
    QVector<int> usedPortVector;
    for (int i = 0; i < d->_pMaxLinkPortCount; i++)
    {
        if (d->_currentLinkPortState[i])
        {
            usedPortVector.append(i);
        }
    }
    return usedPortVector;
}

int ElaGraphicsItem::getUnusedLinkPortCount() const
{
    Q_D(const ElaGraphicsItem);
    return d->_pMaxLinkPortCount - getUsedLinkPortCount();
}

QVector<int> ElaGraphicsItem::getUnusedLinkPort() const
{
    Q_D(const ElaGraphicsItem);
    QVector<int> unusedPortVector;
    for (int i = 0; i < d->_pMaxLinkPortCount; i++)
    {
        if (!(d->_currentLinkPortState[i]))
        {
            unusedPortVector.append(i);
        }
    }
    return unusedPortVector;
}

QRectF ElaGraphicsItem::boundingRect() const
{
    return QRect(-d_ptr->_pWidth / 2, -d_ptr->_pHeight / 2, d_ptr->_pWidth, d_ptr->_pHeight);
}

void ElaGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_D(ElaGraphicsItem);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    if (isSelected())
    {
        painter->drawImage(boundingRect(), d->_pItemSelectedImage);
    }
    else
    {
        painter->drawImage(boundingRect(), d->_pItemImage);
    }
    painter->restore();
}

QDataStream& operator<<(QDataStream& stream, const ElaGraphicsItem* item)
{
    stream << item->x();
    stream << item->y();
    stream << item->zValue();
    stream << item->getWidth();
    stream << item->getHeight();
    stream << item->d_func();
    return stream;
}

QDataStream& operator>>(QDataStream& stream, ElaGraphicsItem* item)
{
    qreal itemX;
    qreal itemY;
    qreal itemZ;
    int itemWidth;
    int itemHeight;
    stream >> itemX >> itemY >> itemZ >> itemWidth >> itemHeight;
    item->setX(itemX);
    item->setY(itemY);
    item->setZValue(itemZ);
    item->setWidth(itemWidth);
    item->setHeight(itemHeight);
    stream >> item->d_func();
    return stream;
}
