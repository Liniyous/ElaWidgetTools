#ifndef ELAGRAPHICSITEM_H
#define ELAGRAPHICSITEM_H

#include <QGraphicsObject>
#include <QJsonObject>
#include <QObject>
#include <QPainter>

#include "stdafx.h"
class ElaGraphicsScene;
class ElaGraphicsItemPrivate;
class ELA_EXPORT ElaGraphicsItem : public QGraphicsObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaGraphicsItem)
    Q_PROPERTY_CREATE_Q_H(int, Width)
    Q_PROPERTY_CREATE_Q_H(int, Height)
    Q_PROPERTY_CREATE_Q_H(QImage, ItemImage)
    Q_PROPERTY_CREATE_Q_H(QImage, ItemSelectedImage)
    Q_PROPERTY_CREATE_Q_H(QString, ItemName)
    Q_PROPERTY_CREATE_Q_H(QVariantMap, DataRoutes)
    Q_PROPERTY_CREATE_Q_H(int, MaxLinkPortCount)
    Q_PROPERTY_CREATE_Q_H(int, CurrentLinkPortCount)
public:
    explicit ElaGraphicsItem(QGraphicsItem* parent = nullptr);
    explicit ElaGraphicsItem(int width, int height, QGraphicsItem* parent = nullptr);
    ~ElaGraphicsItem();

    virtual QRectF boundingRect() const override;

    Q_INVOKABLE QString getItemUID() const;
    void setScene(ElaGraphicsScene* scene);
    void setCurrentLinkPortState(bool isFullLink);
    void setCurrentLinkPortState(bool isLink, int portNumber);
    unsigned long long getCurrentLinkPortState() const;
    bool getCurrentLinkPortState(int portNumber) const;
    int getUnusedLinkPortCount() const;
    QVector<int> getUnusedLinkPort() const;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    friend QDataStream& operator<<(QDataStream& stream, const ElaGraphicsItem* item);
    friend QDataStream& operator>>(QDataStream& stream, ElaGraphicsItem* item);
};

#endif // ELAGRAPHICSITEM_H
