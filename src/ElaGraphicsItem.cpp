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
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsItem, int, MaxLinkPortCount)
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsItem, int, CurrentLinkPortCount)
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
    d->_pCurrentLinkPortCount = 0;
}

ElaGraphicsItem::ElaGraphicsItem(int width, int height, QGraphicsItem* parent)
    : QGraphicsObject(parent), d_ptr(new ElaGraphicsItemPrivate())
{
    Q_D(ElaGraphicsItem);
    d->q_ptr = this;
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | ItemAcceptsInputMethod);
    d->_pWidth = width;
    d->_pHeight = height;
    d->_itemUID = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    d->_pItemImage = QImage(":/include/Image/Moon.jpg");
    d->_pItemSelectedImage = QImage(":/include/Image/Cirno.jpg");
    d->_pItemName = "";
    d->_pMaxLinkPortCount = 1;
    d->_pCurrentLinkPortCount = 0;
}

ElaGraphicsItem::~ElaGraphicsItem()
{
}

QRectF ElaGraphicsItem::boundingRect() const
{
    return QRect(-d_ptr->_pWidth / 2, -d_ptr->_pHeight / 2, d_ptr->_pWidth, d_ptr->_pHeight);
}

QString ElaGraphicsItem::getItemUID() const
{
    return d_ptr->_itemUID;
}

void ElaGraphicsItem::setScene(ElaGraphicsScene* scene)
{
    Q_D(ElaGraphicsItem);
    d->_scene = scene;
}

void ElaGraphicsItem::setCurrentLinkPortState(bool isFullLink)
{
    Q_D(ElaGraphicsItem);
    if (d->_scene->getIsCheckLinkPort())
    {
        if (isFullLink)
        {
            d->_currentLinkPortState = 0;
            d->_currentLinkPortState = ~d->_currentLinkPortState;
        }
        else
        {
            d->_currentLinkPortState = 0;
        }
    }
}

void ElaGraphicsItem::setCurrentLinkPortState(bool isLink, int portNumber)
{
    Q_D(ElaGraphicsItem);
    if (d->_scene->getIsCheckLinkPort())
    {
        if (isLink)
        {
            d->_currentLinkPortState |= (1LL << portNumber);
        }
        else
        {
            d->_currentLinkPortState &= ~(1LL << portNumber);
        }
    }
}

unsigned long long ElaGraphicsItem::getCurrentLinkPortState() const
{
    return d_ptr->_currentLinkPortState;
}

bool ElaGraphicsItem::getCurrentLinkPortState(int portNumber) const
{
    return d_ptr->_currentLinkPortState & (1LL << portNumber);
}

int ElaGraphicsItem::getUnusedLinkPortCount() const
{
    if (d_ptr->_scene->getIsCheckLinkPort())
    {
        return d_ptr->_pMaxLinkPortCount - d_ptr->_pCurrentLinkPortCount;
    }
    return 0;
}

QVector<int> ElaGraphicsItem::getUnusedLinkPort() const
{
    QVector<int> unusedPortVector;
    if (d_ptr->_scene->getIsCheckLinkPort())
    {
        for (int i = 0; i < d_ptr->_pMaxLinkPortCount; i++)
        {
            if (!(d_ptr->_currentLinkPortState & (1LL << i)))
            {
                unusedPortVector.append(i);
            }
        }
    }
    return unusedPortVector;
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
    qreal itemWidth;
    qreal itemHeight;
    stream >> itemX >> itemY >> itemZ >> itemWidth >> itemHeight;
    item->setX(itemX);
    item->setY(itemY);
    item->setZValue(itemZ);
    item->setWidth(itemWidth);
    item->setHeight(itemHeight);
    stream >> item->d_func();
    return stream;
}
