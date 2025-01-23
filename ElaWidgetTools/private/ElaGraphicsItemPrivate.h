#ifndef ELAGRAPHICSITEMPRIVATE_H
#define ELAGRAPHICSITEMPRIVATE_H

#include <QObject>
#include <QVector>

#include "ElaGraphicsItem.h"
class ElaGraphicsItemPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaGraphicsItem)
    Q_PROPERTY_CREATE_D(int, Width)
    Q_PROPERTY_CREATE_D(int, Height)
    Q_PROPERTY_CREATE_D(QImage, ItemImage)
    Q_PROPERTY_CREATE_D(QImage, ItemSelectedImage)
    Q_PROPERTY_CREATE_D(QString, ItemName)
    Q_PROPERTY_CREATE_D(QVariantMap, DataRoutes)
    Q_PROPERTY_CREATE_D(int, MaxLinkPortCount)
public:
    explicit ElaGraphicsItemPrivate(QObject* parent = nullptr);
    ~ElaGraphicsItemPrivate();

    friend QDataStream& operator<<(QDataStream& stream, const ElaGraphicsItemPrivate* data);
    friend QDataStream& operator>>(QDataStream& stream, ElaGraphicsItemPrivate* data);

private:
    QString _itemUID;
    QVector<bool> _currentLinkPortState;
};

#endif // ELAGRAPHICSITEMPRIVATE_H
