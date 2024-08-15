#include "ElaGraphicsScene.h"

#include <QDebug>
#include <QFile>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>

#include "ElaGraphicsItem.h"
#include "ElaGraphicsLineItem.h"
#include "private/ElaGraphicsScenePrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsScene, bool, IsAutoSobel)
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsScene, bool, IsCheckLinkPort)
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsScene, QString, SerializePath)
ElaGraphicsScene::ElaGraphicsScene(QObject* parent)
    : QGraphicsScene(parent), d_ptr(new ElaGraphicsScenePrivate())
{
    Q_D(ElaGraphicsScene);
    d->q_ptr = this;
    setItemIndexMethod(QGraphicsScene::NoIndex);
    // qDebug() << this->ElaGraphicsScene::metaObject()->classInfo(6).value();
    d->_pIsAutoSobel = false;
    d->_pIsCheckLinkPort = false;
    d->_sceneMode = ElaGraphicsSceneType::SceneMode::Default;
    d->_pSerializePath = "./scene.bin";
}

ElaGraphicsScene::~ElaGraphicsScene()
{
}

void ElaGraphicsScene::addItem(ElaGraphicsItem* item)
{
    Q_D(ElaGraphicsScene);
    if (!item)
    {
        return;
    }
    for (const auto& pair : d->_items.toStdMap())
    {
        if (pair.second == item)
        {
            return;
        }
    }
    item->setParent(this);
    item->setScene(this);
    item->setZValue(d->_currentZ);
    if (item->getItemName().isEmpty())
    {
        item->setItemName(QString("ElaItem%1").arg(d->_currentZ));
    }
    item->setPos(sceneRect().width() / 2, sceneRect().height() / 2);
    QGraphicsScene::addItem(item);
    d->_currentZ++;
    d->_items.insert(item->getItemUID(), item);
}

void ElaGraphicsScene::removeItem(ElaGraphicsItem* item)
{
    Q_D(ElaGraphicsScene);
    if (!item)
    {
        return;
    }
    d->_items.remove(d->_items.key(item));
    removeItemLink(item);
    QGraphicsScene::removeItem(item);
    delete item;
    update();
}

void ElaGraphicsScene::removeSelectedItems()
{
    QList<ElaGraphicsItem*> selectedItemList = getSelectedElaItems();
    if (selectedItemList.count() == 0)
    {
        return;
    }
    for (auto item : selectedItemList)
    {
        removeItem(item);
    }
}

void ElaGraphicsScene::clear()
{
    Q_D(ElaGraphicsScene);
    d->_itemsLink.clear();
    for (const auto& pair : d->_items.toStdMap())
    {
        delete pair.second;
    }
    d->_items.clear();
    update();
}

QList<ElaGraphicsItem*> ElaGraphicsScene::createAndAddItem(int width, int height, int count)
{
    if (count <= 0)
    {
        return QList<ElaGraphicsItem*>();
    }
    QList<ElaGraphicsItem*> createItemList;
    for (int i = 0; i < count; i++)
    {
        ElaGraphicsItem* item = new ElaGraphicsItem();
        item->setWidth(width);
        item->setHeight(height);
        createItemList.append(item);
        addItem(item);
    }
    return createItemList;
}

QList<ElaGraphicsItem*> ElaGraphicsScene::getSelectedElaItems() const
{
    QList<QGraphicsItem*> selectedItemList = selectedItems();
    QList<ElaGraphicsItem*> selectedElaItemList;
    for (auto item : selectedItemList)
    {
        ElaGraphicsItem* itemCast = dynamic_cast<ElaGraphicsItem*>(item);
        if (itemCast)
        {
            selectedElaItemList.append(itemCast);
        }
    }
    return selectedElaItemList;
}

QList<ElaGraphicsItem*> ElaGraphicsScene::getElaItems(QPoint pos)
{
    QList<QGraphicsItem*> itemList = items(pos);
    QList<ElaGraphicsItem*> elaItemList;
    for (auto item : itemList)
    {
        ElaGraphicsItem* elaItem = dynamic_cast<ElaGraphicsItem*>(item);
        if (elaItem)
        {
            elaItemList.append(elaItem);
        }
    }
    return elaItemList;
}

QList<ElaGraphicsItem*> ElaGraphicsScene::getElaItems(QPointF pos)
{
    QList<QGraphicsItem*> itemList = items(pos);
    QList<ElaGraphicsItem*> elaItemList;
    for (auto item : itemList)
    {
        ElaGraphicsItem* elaItem = dynamic_cast<ElaGraphicsItem*>(item);
        if (elaItem)
        {
            elaItemList.append(elaItem);
        }
    }
    return elaItemList;
}

void ElaGraphicsScene::setSceneMode(ElaGraphicsSceneType::SceneMode mode)
{
    Q_D(ElaGraphicsScene);
    d->_sceneMode = mode;
    if (mode == ElaGraphicsSceneType::SceneMode::DragMove)
    {
        views().at(0)->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else
    {
        views().at(0)->setDragMode(QGraphicsView::RubberBandDrag);
    }
}

ElaGraphicsSceneType::SceneMode ElaGraphicsScene::getSceneMode() const
{
    return d_ptr->_sceneMode;
}

void ElaGraphicsScene::selectAllItems()
{
    Q_D(ElaGraphicsScene);
    for (const auto& pair : d->_items.toStdMap())
    {
        ElaGraphicsItem* item = pair.second;
        item->setSelected(true);
    }
}

QList<QVariantMap> ElaGraphicsScene::getItemLinkList() const
{
    return d_ptr->_itemsLink;
}

bool ElaGraphicsScene::addItemLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2, int port1, int port2)
{
    Q_D(ElaGraphicsScene);
    if (!item1 || !item2 || (item1 == item2) || port1 < 0 || port2 < 0 || item1->getMaxLinkPortCount() <= port1 || item2->getMaxLinkPortCount() <= port2)
    {
        return false;
    }
    if (d->_pIsCheckLinkPort)
    {
        if (!item1->getCurrentLinkPortState(port1) && !item2->getCurrentLinkPortState(port2))
        {
            item1->setCurrentLinkPortState(true, port1);
            item1->setCurrentLinkPortCount(item1->getCurrentLinkPortCount() + 1);
            item2->setCurrentLinkPortState(true, port2);
            item2->setCurrentLinkPortCount(item2->getCurrentLinkPortCount() + 1);
        }
        else
        {
            return false;
        }
    }
    QVariantMap linkObject;
    linkObject.insert(item1->getItemUID(), port1);
    linkObject.insert(item2->getItemUID(), port2);
    d->_itemsLink.append(linkObject);
    ElaGraphicsLineItem* lineItem = new ElaGraphicsLineItem(item1, item2, port1, port2);
    QGraphicsScene::addItem(lineItem);
    d->_lineItemsList.append(lineItem);
    update();
    return true;
}

bool ElaGraphicsScene::removeItemLink(ElaGraphicsItem* item1)
{
    Q_D(ElaGraphicsScene);
    if (!item1)
    {
        return false;
    }
    if (d->_pIsCheckLinkPort)
    {
        item1->setCurrentLinkPortState(false);
        item1->setCurrentLinkPortCount(0);
    }
    // 处理与该Item有关的连接
    foreach (auto& link, d->_itemsLink)
    {
        if (link.contains(item1->getItemUID()))
        {
            if (d->_pIsCheckLinkPort)
            {
                // 解除otherItem端口占用
                QStringList keys = link.keys();
                keys.removeOne(item1->getItemUID());
                ElaGraphicsItem* otherItem = d->_items.value(keys.at(0));
                otherItem->setCurrentLinkPortState(false, link.value(keys.at(0)).toInt());
                otherItem->setCurrentLinkPortCount(otherItem->getCurrentLinkPortCount() - 1);
            }
            d->_itemsLink.removeOne(link);
        }
    }
    // 处理连接图元
    foreach (auto lineItem, d->_lineItemsList)
    {
        if (lineItem->isTargetLink(item1))
        {
            d->_lineItemsList.removeOne(lineItem);
            QGraphicsScene::removeItem(lineItem);
            delete lineItem;
        }
    }
    update();
    return true;
}

bool ElaGraphicsScene::removeItemLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2, int port1, int port2)
{
    Q_D(ElaGraphicsScene);
    if (!item1 || !item2)
    {
        return false;
    }
    bool isLinkExist = false;
    foreach (auto& link, d->_itemsLink)
    {
        if (link.value(item1->getItemUID()) == port1 && link.value(item2->getItemUID()) == port2)
        {
            d->_itemsLink.removeOne(link);
            // 这里处理连线图元
            isLinkExist = true;
            break;
        }
    }
    foreach (auto lineItem, d->_lineItemsList)
    {
        if (lineItem->isTargetLink(item1, item2, port1, port2))
        {
            d->_lineItemsList.removeOne(lineItem);
            QGraphicsScene::removeItem(lineItem);
            delete lineItem;
            break;
        }
    }
    if (isLinkExist)
    {
        if (d->_pIsCheckLinkPort)
        {
            item1->setCurrentLinkPortState(false, port1);
            item1->setCurrentLinkPortCount(item1->getCurrentLinkPortCount() - 1);
            item2->setCurrentLinkPortState(false, port2);
            item2->setCurrentLinkPortCount(item2->getCurrentLinkPortCount() - 1);
        }
        update();
        return true;
    }
    else
    {
        return false;
    }
}

QVector<QVariantMap> ElaGraphicsScene::getItemsDataRoute() const
{
    QVector<QVariantMap> dataRouteVector;
    for (const auto& pair : d_ptr->_items.toStdMap())
    {
        ElaGraphicsItem* item = pair.second;
        dataRouteVector.append(item->getDataRoutes());
    }
    return dataRouteVector;
}

void ElaGraphicsScene::serialize()
{
    Q_D(ElaGraphicsScene);
    QFile file(d->_pSerializePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "serialize Error";
        return;
    }
    QDataStream serialStream(&file);
    serialStream << d;
    file.flush();
    file.close();
}

void ElaGraphicsScene::deserialize()
{
    Q_D(ElaGraphicsScene);
    QFile file(d->_pSerializePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "deserialize Error";
        return;
    }
    QDataStream deserialStream(&file);
    deserialStream >> d;
    file.close();
    update();
}

void ElaGraphicsScene::focusOutEvent(QFocusEvent* event)
{
    Q_D(ElaGraphicsScene);
    d->_sceneMode = ElaGraphicsSceneType::SceneMode::Default;
    d->_removeLinkLineItem();
    QGraphicsScene::focusOutEvent(event);
}

void ElaGraphicsScene::keyPressEvent(QKeyEvent* event)
{
    Q_D(ElaGraphicsScene);
    switch (event->key())
    {
    case Qt::Key_Control:
    {
        d->_sceneMode = ElaGraphicsSceneType::SceneMode::MultiSelect;
        break;
    }
    case Qt::Key_Shift:
    {
        d->_sceneMode = ElaGraphicsSceneType::SceneMode::ItemLink;
        clearSelection();
        break;
    }
    case Qt::Key_Delete:
    {
        removeSelectedItems();
        break;
    }
    }
    QGraphicsScene::keyPressEvent(event);
}

void ElaGraphicsScene::keyReleaseEvent(QKeyEvent* event)
{
    Q_D(ElaGraphicsScene);
    d->_sceneMode = ElaGraphicsSceneType::SceneMode::Default;
    d->_removeLinkLineItem();
    QGraphicsScene::keyReleaseEvent(event);
}

void ElaGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_D(ElaGraphicsScene);
    if (event->button() == Qt::LeftButton)
    {
        if (d->_sceneMode == ElaGraphicsSceneType::SceneMode::Default)
        {
            d->_lastPos = event->pos();
        }
    }
    QList<QGraphicsItem*> selectedItemList = selectedItems();
    QGraphicsScene::mousePressEvent(event);
    if (d->_sceneMode == ElaGraphicsSceneType::SceneMode::ItemLink)
    {
        for (auto item : selectedItemList)
        {
            item->setSelected(true);
        }
        d->_lastPos = event->pos();
    }
}

void ElaGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    Q_D(ElaGraphicsScene);
    QList<ElaGraphicsItem*> itemList = getElaItems(event->scenePos());
    if (event->button() == Qt::LeftButton)
    {
        switch (d->_sceneMode)
        {
        case ElaGraphicsSceneType::SceneMode::Default:
        {
            if (itemList.count() > 0)
            {
                Q_EMIT mouseLeftClickedItem(itemList[0]);
            }
            break;
        }
        case ElaGraphicsSceneType::SceneMode::ItemLink:
        {
            QList<ElaGraphicsItem*> selectedItemList = getSelectedElaItems();
            if (selectedItemList.count() == 1 && !d->_linkLineItem)
            {
                d->_linkLineItem = new ElaGraphicsLineItem(selectedItemList.at(0)->pos(), selectedItemList.at(0)->pos());
                QGraphicsScene::addItem(d->_linkLineItem);
            }
            else if (selectedItemList.count() == 2)
            {
                if (d->_pIsCheckLinkPort)
                {
                    Q_EMIT showItemLink();
                }
                else
                {
                    QVariantMap linkObject;
                    for (auto item : selectedItemList)
                    {
                        linkObject.insert(item->getItemUID(), 0);
                    }
                    d->_itemsLink.append(linkObject);
                    QGraphicsScene::mouseReleaseEvent(event);
                    d->_removeLinkLineItem();
                    addItemLink(selectedItemList.at(0), selectedItemList.at(1));
                    clearSelection();
                    return;
                }
            }
            else
            {
                d->_removeLinkLineItem();
                update();
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        if (itemList.count() > 0)
        {
            clearSelection();
            itemList[0]->setSelected(true);
            Q_EMIT mouseRightClickedItem(itemList[0]);
        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void ElaGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    Q_D(ElaGraphicsScene);
    if (d->_sceneMode == ElaGraphicsSceneType::SceneMode::ItemLink)
    {
        if (getSelectedElaItems().count() == 1)
        {
            if (d->_linkLineItem)
            {
                d->_linkLineItem->setEndPoint(event->scenePos());
                d->_linkLineItem->update();
            }
        }
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void ElaGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    QList<ElaGraphicsItem*> itemList = getElaItems(event->scenePos());
    if (itemList.count() > 0)
    {
        Q_EMIT mouseDoubleClickedItem(itemList[0]);
    }
    QGraphicsScene::mouseDoubleClickEvent(event);
}
