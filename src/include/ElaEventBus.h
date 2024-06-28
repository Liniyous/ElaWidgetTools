#ifndef ELAEVENTBUS_H
#define ELAEVENTBUS_H

#include <QObject>
#include <QVariantMap>

#include "Def.h"
#include "singleton.h"
#include "stdafx.h"
class ElaEventPrivate;
class ELA_EXPORT ElaEvent : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaEvent)
    Q_PROPERTY_CREATE_Q_H(QString, EventName);
    Q_PROPERTY_CREATE_Q_H(QString, FunctionName);
    Q_PROPERTY_CREATE_Q_H(Qt::ConnectionType, ConnectionType);

public:
    explicit ElaEvent(QObject* parent = nullptr);
    explicit ElaEvent(QString eventName, QString functionName, QObject* parent = nullptr);
    ~ElaEvent();
    ElaEventBusType::EventBusReturnType registerAndInit();
};

class ElaEventBusPrivate;
class ELA_EXPORT ElaEventBus : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaEventBus)
    Q_SINGLETON_CREATE_H(ElaEventBus);

private:
    explicit ElaEventBus(QObject* parent = nullptr);
    ~ElaEventBus();

public:
    ElaEventBusType::EventBusReturnType post(const QString& eventName, const QVariantMap& data = {});
    QStringList getRegisteredEventsName() const;

private:
    friend class ElaEvent;
};

#endif // ELAEVENTBUS_H
