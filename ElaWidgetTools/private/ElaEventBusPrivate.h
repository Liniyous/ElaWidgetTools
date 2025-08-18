#ifndef ELAEVENTBUSPRIVATE_H
#define ELAEVENTBUSPRIVATE_H

#include <QMap>
#include <QObject>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaEvent;
class ElaEventPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaEvent)
    Q_PROPERTY_CREATE_D(QString, EventName);
    Q_PROPERTY_CREATE_D(QString, FunctionName);
    Q_PROPERTY_CREATE_D(Qt::ConnectionType, ConnectionType);

public:
    explicit ElaEventPrivate(QObject* parent = nullptr);
    ~ElaEventPrivate();
};

class ElaEventBus;
class ElaEventBusPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaEventBus)
public:
    explicit ElaEventBusPrivate(QObject* parent = nullptr);
    ~ElaEventBusPrivate();
    ElaEventBusType::EventBusReturnType registerEvent(ElaEvent* event);
    void unRegisterEvent(ElaEvent* event);

private:
    QMap<QString, QList<ElaEvent*>> _eventMap;
};

#endif // ELAEVENTBUSPRIVATE_H
