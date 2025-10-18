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
    if (d->_routeList.count() == 0)
    {
        Q_EMIT navigationRouterStateChanged(ElaNavigationRouterType::BackValid);
        Q_EMIT navigationRouterStateChanged(ElaNavigationRouterType::ForwardInvalid);
    }
    else
    {
        if (d->_routeList.count() >= d->_pMaxRouteCount)
        {
            d->_routeList.removeFirst();
            d->_currentIndex -= 1;
        }
    }
    QVariantMap saveData;
    saveData.insert("ElaRouteObject", QVariant::fromValue<QObject*>(routeObject));
    saveData.insert("ElaRouteFunctionName", routeFunctionName);
    saveData.insert("ElaRouteData", routeData);
    saveData.insert("ElaRouteConnectionType", QVariant::fromValue<Qt::ConnectionType>(connectionType));
    if (d->_currentIndex != d->_routeList.count() - 1)
    {
        if (d->_currentIndex == -1)
        {
            d->_routeList.clear();
        }
        else
        {
            d->_routeList.remove(d->_currentIndex + 1, d->_routeList.count() - d->_currentIndex - 1);
        }
        Q_EMIT navigationRouterStateChanged(ElaNavigationRouterType::ForwardInvalid);
    }
    if (d->_currentIndex <= 0)
    {
        Q_EMIT navigationRouterStateChanged(ElaNavigationRouterType::BackValid);
        Q_EMIT navigationRouterStateChanged(ElaNavigationRouterType::ForwardInvalid);
    }
    d->_routeList.append(saveData);
    d->_currentIndex = d->_routeList.count() - 1;
    return ElaNavigationRouterType::Success;
}

void ElaNavigationRouter::clearNavigationRoute()
{
    Q_D(ElaNavigationRouter);
    d->_currentIndex = -1;
    d->_routeList.clear();
    Q_EMIT navigationRouterStateChanged(ElaNavigationRouterType::BackInvalid);
    Q_EMIT navigationRouterStateChanged(ElaNavigationRouterType::ForwardValid);
}

void ElaNavigationRouter::navigationRouteBack()
{
    Q_D(ElaNavigationRouter);
    if (d->_routeList.isEmpty())
    {
        return;
    }
    if (d->_currentIndex == 0)
    {
        Q_EMIT navigationRouterStateChanged(ElaNavigationRouterType::BackInvalid);
    }
    if (d->_currentIndex == d->_routeList.size() - 1)
    {
        Q_EMIT navigationRouterStateChanged(ElaNavigationRouterType::ForwardValid);
    }
    QVariantMap routeSaveData = d->_routeList[d->_currentIndex];
    d->_currentIndex -= 1;
    QObject* routeObject = routeSaveData.value("ElaRouteObject").value<QObject*>();
    QString routeFunctionName = routeSaveData.value("ElaRouteFunctionName").toString();
    QVariantMap routeData = routeSaveData.value("ElaRouteData").toMap();
    routeData.insert("ElaRouteBackMode", true);
    Qt::ConnectionType connectionType = routeSaveData.value("ElaRouteConnectionType").value<Qt::ConnectionType>();
    QMetaObject::invokeMethod(routeObject, routeFunctionName.toLocal8Bit().constData(), connectionType, Q_ARG(QVariantMap, routeData));
}

void ElaNavigationRouter::navigationRouteForward()
{
    Q_D(ElaNavigationRouter);
    if (d->_routeList.isEmpty())
    {
        return;
    }
    if (d->_currentIndex <= 0)
    {
        Q_EMIT navigationRouterStateChanged(ElaNavigationRouterType::BackValid);
    }
    if (d->_currentIndex == d->_routeList.size() - 2)
    {
        Q_EMIT navigationRouterStateChanged(ElaNavigationRouterType::ForwardInvalid);
    }
    if (d->_currentIndex < d->_routeList.size() - 1)
    {
        d->_currentIndex += 1;
    }
    QVariantMap routeSaveData = d->_routeList[d->_currentIndex];
    QObject* routeObject = routeSaveData.value("ElaRouteObject").value<QObject*>();
    QString routeFunctionName = routeSaveData.value("ElaRouteFunctionName").toString();
    QVariantMap routeData = routeSaveData.value("ElaRouteData").toMap();
    routeData.insert("ElaRouteBackMode", false);
    Qt::ConnectionType connectionType = routeSaveData.value("ElaRouteConnectionType").value<Qt::ConnectionType>();
    QMetaObject::invokeMethod(routeObject, routeFunctionName.toLocal8Bit().constData(), connectionType, Q_ARG(QVariantMap, routeData));
}
