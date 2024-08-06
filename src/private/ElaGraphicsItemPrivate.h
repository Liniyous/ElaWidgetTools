#ifndef ELAGRAPHICSITEMPRIVATE_H
#define ELAGRAPHICSITEMPRIVATE_H

#include <QObject>

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
    Q_PROPERTY_CREATE_D(int, CurrentLinkPortCount)
public:
    explicit ElaGraphicsItemPrivate(QObject* parent = nullptr);
    ~ElaGraphicsItemPrivate();

    friend QDataStream& operator<<(QDataStream& stream, const ElaGraphicsItemPrivate* data);
    friend QDataStream& operator>>(QDataStream& stream, ElaGraphicsItemPrivate* data);

private:
    QString _itemUID;
    unsigned long long _currentLinkPortState{0};
    ElaGraphicsScene* _scene{nullptr};
};

#endif // ELAGRAPHICSITEMPRIVATE_H
