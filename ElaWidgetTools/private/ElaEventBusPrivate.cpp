#include "ElaEventBusPrivate.h"

#include "ElaEventBus.h"
ElaEventPrivate::ElaEventPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaEventPrivate::~ElaEventPrivate()
{
}

ElaEventBusPrivate::ElaEventBusPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaEventBusPrivate::~ElaEventBusPrivate()
{
}

ElaEventBusType::EventBusReturnType ElaEventBusPrivate::registerEvent(ElaEvent* event)
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
        if (eventList.contains(event))
        {
            return ElaEventBusType::EventBusReturnType::EventInvalid;
        }
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

void ElaEventBusPrivate::unRegisterEvent(ElaEvent* event)
{
    if (!event)
    {
        return;
    }
    if (event->getEventName().isEmpty())
    {
        return;
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
}
