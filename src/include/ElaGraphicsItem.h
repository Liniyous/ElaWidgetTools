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
public:
    explicit ElaGraphicsItem(QGraphicsItem* parent = nullptr);
    explicit ElaGraphicsItem(int width, int height, QGraphicsItem* parent = nullptr);
    ~ElaGraphicsItem();

    QString getItemUID() const;

    void setLinkPortState(bool isFullLink);
    void setLinkPortState(bool isLink, int portIndex);

    bool getLinkPortState(int portIndex) const;
    QVector<bool> getLinkPortState() const;

    int getUsedLinkPortCount() const;
    QVector<int> getUsedLinkPort() const;
    int getUnusedLinkPortCount() const;
    QVector<int> getUnusedLinkPort() const;

protected:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    friend QDataStream& operator<<(QDataStream& stream, const ElaGraphicsItem* item);
    friend QDataStream& operator>>(QDataStream& stream, ElaGraphicsItem* item);
};

#endif // ELAGRAPHICSITEM_H
