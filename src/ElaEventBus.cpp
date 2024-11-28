#include "ElaEventBus.h"

#include <QVariant>

#include "ElaEventBusPrivate.h"
Q_SINGLETON_CREATE_CPP(ElaEventBus);
Q_PROPERTY_CREATE_Q_CPP(ElaEvent, QString, EventName);
Q_PROPERTY_CREATE_Q_CPP(ElaEvent, QString, FunctionName);
Q_PROPERTY_CREATE_Q_CPP(ElaEvent, Qt::ConnectionType, ConnectionType);
ElaEvent::ElaEvent(QObject* parent)
    : QObject{parent}, d_ptr(new ElaEventPrivate())
{
    Q_D(ElaEvent);
    d->q_ptr = this;
    d->_pConnectionType = Qt::AutoConnection;
    d->_pFunctionName = "";
    d->_pEventName = "";
}

ElaEvent::ElaEvent(QString eventName, QString functionName, QObject* parent)
    : QObject{parent}, d_ptr(new ElaEventPrivate())
{
    Q_D(ElaEvent);
    d->q_ptr = this;
    d->_pConnectionType = Qt::AutoConnection;
    d->_pEventName = eventName;
    d->_pFunctionName = functionName;
}

ElaEventBusType::EventBusReturnType ElaEvent::registerAndInit()
{
    return ElaEventBus::getInstance()->d_ptr->registerEvent(this);
}

ElaEvent::~ElaEvent()
{
    ElaEventBus::getInstance()->d_ptr->unRegisterEvent(this);
}

ElaEventBus::ElaEventBus(QObject* parent)
    : QObject{parent}, d_ptr(new ElaEventBusPrivate())
{
    Q_D(ElaEventBus);
    d->q_ptr = this;
}

ElaEventBus::~ElaEventBus()
{
}

ElaEventBusType::EventBusReturnType ElaEventBus::post(const QString& eventName, const QVariantMap& data)
{
    Q_D(ElaEventBus);
    if (eventName.isEmpty())
    {
        return ElaEventBusType::EventBusReturnType::EventNameInvalid;
    }
    if (d->_eventMap.contains(eventName))
    {
        QList<ElaEvent*> eventList = d->_eventMap.value(eventName);
        foreach (auto event, eventList)
        {
            if (event->parent())
            {
                QMetaObject::invokeMethod(event->parent(), event->getFunctionName().toLocal8Bit().constData(), event->getConnectionType(), Q_ARG(QVariantMap, data));
            }
        }
    }
    return ElaEventBusType::EventBusReturnType::Success;
}

QStringList ElaEventBus::getRegisteredEventsName() const
{
    Q_D(const ElaEventBus);
    if (d->_eventMap.count() == 0)
    {
        return QStringList();
    }
    QStringList eventsNameList = d->_eventMap.keys();
    return eventsNameList;
}
