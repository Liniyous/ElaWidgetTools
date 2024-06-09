#ifndef ELAEVENTBUS_H
#define ELAEVENTBUS_H
#include <QMap>
#include <QObject>

#include "Def.h"
#include "singleton.h"
#include "stdafx.h"
class ELA_EXPORT ElaEvent : public QObject
{
    Q_OBJECT
    Q_PROPERTY_CREATE(QString, EventName);

public:
    explicit ElaEvent(QObject* parent = nullptr);
    explicit ElaEvent(QString eventName, QObject* parent = nullptr);
    ~ElaEvent();
Q_SIGNALS:
    Q_SIGNAL void triggered(QMap<QString, QVariant> data);
};

class ELA_EXPORT ElaEventBus : public QObject
{
    Q_OBJECT

private:
    explicit ElaEventBus(QObject* parent = nullptr);
    ~ElaEventBus();

public:
    Q_SINGLETON_CREATE(ElaEventBus);
    ElaEventBusType::EventBusReturnType registerEvent(ElaEvent* event);
    ElaEventBusType::EventBusReturnType unRegisterEvent(ElaEvent* event);
    ElaEventBusType::EventBusReturnType post(const QString& eventName, const QMap<QString, QVariant>& data = {});
    QStringList getRegisteredEventsName();

private:
    QMap<QString, QList<ElaEvent*>> _eventMap;
};

#endif // ELAEVENTBUS_H
