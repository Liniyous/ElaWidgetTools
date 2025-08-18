#ifndef ELAGRAPHICSSCENE_H
#define ELAGRAPHICSSCENE_H

#include <QDataStream>
#include <QEvent>
#include <QGraphicsScene>
#include <QJsonObject>
#include <QObject>
#include <QPainter>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaGraphicsItem;
class ElaGraphicsScenePrivate;
class ELA_EXPORT ElaGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
    Q_Q_CREATE(ElaGraphicsScene)
    Q_PROPERTY_CREATE_Q_H(bool, IsCheckLinkPort)
    Q_PROPERTY_CREATE_Q_H(QString, SerializePath)
public:
    explicit ElaGraphicsScene(QObject* parent = nullptr);
    ~ElaGraphicsScene();
    void addItem(ElaGraphicsItem* item);
    void removeItem(ElaGraphicsItem* item);
    void removeSelectedItems();
    void clear();

    QList<ElaGraphicsItem*> createAndAddItem(int width, int height, int count = 1);
    QList<ElaGraphicsItem*> getSelectedElaItems() const;
    QList<ElaGraphicsItem*> getElaItems();
    QList<ElaGraphicsItem*> getElaItems(QPoint pos);
    QList<ElaGraphicsItem*> getElaItems(QPointF pos);
    QList<ElaGraphicsItem*> getElaItems(QRect rect);
    QList<ElaGraphicsItem*> getElaItems(QRectF rect);

    void setSceneMode(ElaGraphicsSceneType::SceneMode mode);
    ElaGraphicsSceneType::SceneMode getSceneMode() const;

    void selectAllItems();

    QList<QVariantMap> getItemLinkList() const;
    bool addItemLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2, int port1 = 0, int port2 = 0);
    bool removeItemLink(ElaGraphicsItem* item1);
    bool removeItemLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2, int port1 = 0, int port2 = 0);

    QVector<QVariantMap> getItemsDataRoute() const;

    // 序列化 反序列化
    void serialize();
    void deserialize();

Q_SIGNALS:
    void showItemLink();
    void mouseLeftClickedItem(ElaGraphicsItem* item);
    void mouseRightClickedItem(ElaGraphicsItem* item);
    void mouseDoubleClickedItem(ElaGraphicsItem* item);

protected:
    virtual void focusOutEvent(QFocusEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // ELAGRAPHICSSCENE_H
