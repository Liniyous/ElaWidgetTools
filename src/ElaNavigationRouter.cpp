#include "ElaNavigationRouter.h"

#include "ElaNavigationRouterPrivate.h"

Q_SINGLETON_CREATE_CPP(ElaNavigationRouter)
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationRouter, int, MaxRouteCount)
ElaNavigationRouter::ElaNavigationRouter(QObject* parent)
    : QObject{parent}, d_ptr(new ElaNavigationRouterPrivate())
{
    Q_D(ElaNavigationRouter);
    d->q_ptr = this;
    d->_pMaxRouteCount = 25;
}

ElaNavigationRouter::~ElaNavigationRouter()
{
}

ElaNavigationRouterType::NavigationRouteType ElaNavigationRouter::navigationRoute(QObject* routeObject, QString routeFunctionName, const QVariantMap& routeData, Qt::ConnectionType connectionType)
{
    Q_D(ElaNavigationRouter);
    if (!routeObject)
    {
        return ElaNavigationRouterType::ObjectInvalid;
    }
    if (routeFunctionName.isEmpty())
    {
        return ElaNavigationRouterType::FunctionNameInvalid;
    }
    if (d->_routeQueue.isEmpty())
    {
        Q_EMIT navigationRouterStateChanged(true);
    }
    else
    {
        if (d->_routeQueue.count() >= d->_pMaxRouteCount)
        {
            d->_routeQueue.dequeue();
        }
    }
    QVariantMap saveData;
    saveData.insert("ElaRouteObject", QVariant::fromValue<QObject*>(routeObject));
    saveData.insert("ElaRouteFunctionName", routeFunctionName);
    saveData.insert("ElaRouteData", routeData);
    saveData.insert("ElaRouteConnectionType", QVariant::fromValue<Qt::ConnectionType>(connectionType));
    d->_routeQueue.enqueue(saveData);
    return ElaNavigationRouterType::Success;
}

void ElaNavigationRouter::navigationRouteBack()
{
    Q_D(ElaNavigationRouter);
    int routeQueneCount = d->_routeQueue.count();
    if (routeQueneCount > 0)
    {
        if (routeQueneCount == 1)
        {
            Q_EMIT navigationRouterStateChanged(false);
        }
        QVariantMap routeSaveData = d->_routeQueue.last();
        d->_routeQueue.removeLast();
        QObject* routeObject = routeSaveData.value("ElaRouteObject").value<QObject*>();
        QString routeFunctionName = routeSaveData.value("ElaRouteFunctionName").toString();
        QVariantMap routeData = routeSaveData.value("ElaRouteData").toMap();
        Qt::ConnectionType connectionType = routeSaveData.value("ElaRouteConnectionType").value<Qt::ConnectionType>();
        QMetaObject::invokeMethod(routeObject, routeFunctionName.toLocal8Bit().constData(), connectionType, Q_ARG(QVariantMap, routeData));
    }
}
