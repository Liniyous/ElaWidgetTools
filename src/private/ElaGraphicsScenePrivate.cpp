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

QList<ElaGraphicsItem*> ElaGraphicsScenePrivate::serializeItem(int count)
{
    Q_Q(ElaGraphicsScene);
    QList<ElaGraphicsItem*> itemList;
    for (int i = 0; i < count; i++)
    {
        ElaGraphicsItem* item = new ElaGraphicsItem();
        item->setParent(q);
        item->setScene(q);
        itemList.append(item);
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
    QList<ElaGraphicsItem*> itemList = data->serializeItem(keyList.count());
    for (int i = 0; i < keyList.count(); i++)
    {
        stream >> itemList[i];
    }
    for (int i = 0; i < keyList.count(); i++)
    {
        data->_items.insert(keyList[i], itemList[i]);
    }
    stream >> data->_itemsLink;
    return stream;
}
