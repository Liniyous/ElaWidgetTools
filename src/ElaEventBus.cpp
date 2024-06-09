#include "ElaEventBus.h"

#include <QVariant>
ElaEvent::ElaEvent(QObject* parent)
    : QObject{parent}
{
}

ElaEvent::ElaEvent(QString eventName, QObject* parent)
    : QObject{parent}
{
    this->_pEventName = eventName;
}

ElaEvent::~ElaEvent()
{
    ElaEventBus::getInstance()->unRegisterEvent(this);
}

ElaEventBus::ElaEventBus(QObject* parent)
    : QObject{parent}
{
}

ElaEventBus::~ElaEventBus()
{
}

ElaEventBusType::EventBusReturnType ElaEventBus::registerEvent(ElaEvent* event)
{
    if (!event)
    {
        return ElaEventBusType::EventBusReturnType::EventInvalid;
    }
    if (event->getEventName().isEmpty())
    {
        return ElaEventBusType::EventBusReturnType::EventNameInvalid;
    }
    if (_eventMap.contains(event->getEventName()))
    {
        QList<ElaEvent*> eventList = _eventMap.value(event->getEventName());
        eventList.append(event);
        _eventMap[event->getEventName()] = eventList;
    }
    else
    {
        QList<ElaEvent*> eventList;
        eventList.append(event);
        _eventMap.insert(event->getEventName(), eventList);
    }
    return ElaEventBusType::EventBusReturnType::Success;
}

ElaEventBusType::EventBusReturnType ElaEventBus::unRegisterEvent(ElaEvent* event)
{
    if (!event)
    {
        return ElaEventBusType::EventBusReturnType::EventInvalid;
    }
    if (event->getEventName().isEmpty())
    {
        return ElaEventBusType::EventBusReturnType::EventNameInvalid;
    }
    if (_eventMap.contains(event->getEventName()))
    {
        if (_eventMap[event->getEventName()].count() == 1)
        {
            _eventMap.remove(event->getEventName());
        }
        else
        {
            QList<ElaEvent*> eventList = _eventMap.value(event->getEventName());
            eventList.removeOne(event);
            _eventMap[event->getEventName()] = eventList;
        }
    }
    return ElaEventBusType::EventBusReturnType::Success;
}

ElaEventBusType::EventBusReturnType ElaEventBus::post(const QString& eventName, const QMap<QString, QVariant>& data)
{
    if (eventName.isEmpty())
    {
        return ElaEventBusType::EventBusReturnType::EventNameInvalid;
    }
    if (_eventMap.contains(eventName))
    {
        QList<ElaEvent*> eventList = _eventMap.value(eventName);
        foreach (auto event, eventList)
        {
            Q_EMIT event->triggered(data);
        }
    }
    return ElaEventBusType::EventBusReturnType::Success;
}

QStringList ElaEventBus::getRegisteredEventsName()
{
    if (_eventMap.count() == 0)
    {
        return QStringList();
    }
    QStringList eventsNameList = _eventMap.keys();
    return eventsNameList;
}
