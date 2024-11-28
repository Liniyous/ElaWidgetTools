#include "ElaGraphicsScenePrivate.h"

#include "ElaGraphicsItem.h"
#include "ElaGraphicsLineItem.h"
#include "ElaGraphicsScene.h"
ElaGraphicsScenePrivate::ElaGraphicsScenePrivate(QObject* parent)
    : QObject(parent)
{
}

ElaGraphicsScenePrivate::~ElaGraphicsScenePrivate()
{
}

QDataStream& operator<<(QDataStream& stream, const ElaGraphicsScenePrivate* data)
{
    QStringList keyList = data->_items.keys();
    QList<ElaGraphicsItem*> itemList = data->_items.values();
    stream << keyList;
    for (int i = 0; i < keyList.count(); i++)
    {
        stream << itemList[i];
    }
    stream << data->_itemsLink;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, ElaGraphicsScenePrivate* data)
{
    QStringList keyList;
    stream >> keyList;
    QList<ElaGraphicsItem*> itemList = data->_serializeItem(keyList.count());
    for (int i = 0; i < keyList.count(); i++)
    {
        stream >> itemList[i];
    }
    for (int i = 0; i < keyList.count(); i++)
    {
        data->_items.insert(keyList[i], itemList[i]);
    }
    stream >> data->_itemsLink;
    data->_deserializeLink();
    return stream;
}

QList<ElaGraphicsItem*> ElaGraphicsScenePrivate::_serializeItem(int count)
{
    Q_Q(ElaGraphicsScene);
    QList<ElaGraphicsItem*> itemList;
    for (int i = 0; i < count; i++)
    {
        ElaGraphicsItem* item = new ElaGraphicsItem();
        item->setParent(q);
        itemList.append(item);
        q->QGraphicsScene::addItem(item);
        _currentZ++;
    }
    return itemList;
}

void ElaGraphicsScenePrivate::_removeLinkLineItem()
{
    Q_Q(ElaGraphicsScene);
    if (_linkLineItem)
    {
        q->QGraphicsScene::removeItem(_linkLineItem);
        delete _linkLineItem;
        _linkLineItem = nullptr;
    }
}

void ElaGraphicsScenePrivate::_deserializeLink()
{
    Q_Q(ElaGraphicsScene);
    for (int i = 0; i < _itemsLink.count(); i++)
    {
        QVariantMap itemLinkData = _itemsLink[i];
        QList<QString> uidList = itemLinkData.keys();
        QList<QVariant> portList = itemLinkData.values();
        ElaGraphicsLineItem* lineItem = new ElaGraphicsLineItem(_items.value(uidList[0]), _items.value(uidList[1]), portList[0].toInt(), portList[1].toInt());
        q->QGraphicsScene::addItem(lineItem);
        _lineItemsList.append(lineItem);
    }
}
