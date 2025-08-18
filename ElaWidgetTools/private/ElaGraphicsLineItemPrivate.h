#ifndef ELAGRAPHICSLINEITEMPRIVATE_H
#define ELAGRAPHICSLINEITEMPRIVATE_H

#include <QMap>
#include <QObject>
#include <QPointF>

#include "ElaProperty.h"
class ElaGraphicsItem;
class ElaGraphicsLineItem;
class ElaGraphicsLineItemPrivate
{
    Q_D_CREATE(ElaGraphicsLineItem)
    Q_PRIVATE_CREATE_D(QPointF, StartPoint);
    Q_PRIVATE_CREATE_D(QPointF, EndPoint);
    Q_PRIVATE_CREATE_D(ElaGraphicsItem*, StartItem);
    Q_PRIVATE_CREATE_D(ElaGraphicsItem*, EndItem);
    Q_PRIVATE_CREATE_D(int, StartItemPort);
    Q_PRIVATE_CREATE_D(int, EndItemPort);

public:
    explicit ElaGraphicsLineItemPrivate();
    ~ElaGraphicsLineItemPrivate();

private:
    QMap<ElaGraphicsItem*, int> _linkItemMap;
    bool _isCreateWithItem{true};
};

#endif // ELAGRAPHICSLINEITEMPRIVATE_H
