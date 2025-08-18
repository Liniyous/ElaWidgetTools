#ifndef ELAGRAPHICSSCENEPRIVATE_H
#define ELAGRAPHICSSCENEPRIVATE_H

#include <QMap>
#include <QObject>
#include <QPointF>

#include "ElaGraphicsScene.h"
#include "ElaProperty.h"
class ElaGraphicsItem;
class ElaGraphicsLineItem;
class ElaGraphicsScenePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaGraphicsScene)
    Q_PROPERTY_CREATE_D(bool, IsCheckLinkPort)
    Q_PROPERTY_CREATE_D(QString, SerializePath)
public:
    explicit ElaGraphicsScenePrivate(QObject* parent = nullptr);
    ~ElaGraphicsScenePrivate();

    friend QDataStream& operator<<(QDataStream& stream, const ElaGraphicsScenePrivate* data);
    friend QDataStream& operator>>(QDataStream& stream, ElaGraphicsScenePrivate* data);

private:
    bool _isLeftButtonPress{false};
    QMap<QString, ElaGraphicsItem*> _items; // 存储所有item
    ElaGraphicsSceneType::SceneMode _sceneMode;
    QList<QVariantMap> _itemsLink; // item连接状态
    QList<ElaGraphicsLineItem*> _lineItemsList;
    ElaGraphicsLineItem* _linkLineItem{nullptr};
    qreal _currentZ{1};
    QPointF _lastPos;
    QPointF _lastLeftPressPos;
    QList<ElaGraphicsItem*> _serializeItem(int count);
    void _removeLinkLineItem();
    void _deserializeLink();
};

#endif // ELAGRAPHICSSCENEPRIVATE_H
