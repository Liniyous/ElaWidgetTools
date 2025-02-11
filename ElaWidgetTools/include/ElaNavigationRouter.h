#ifndef ELANAVIGATIONROUTER_H
#define ELANAVIGATIONROUTER_H

#include <QObject>
#include <QVariantMap>

#include "Def.h"
#include "singleton.h"
#include "stdafx.h"

class ElaNavigationRouterPrivate;
class ELA_EXPORT ElaNavigationRouter : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaNavigationRouter)
    Q_SINGLETON_CREATE_H(ElaNavigationRouter)
    Q_PROPERTY_CREATE_Q_H(int, MaxRouteCount)
private:
    explicit ElaNavigationRouter(QObject* parent = nullptr);
    ~ElaNavigationRouter() override;

Q_SIGNALS:
    Q_SIGNAL void navigationRouterStateChanged(bool state);

public:
    ElaNavigationRouterType::NavigationRouteType navigationRoute(QObject* routeObject, QString routeFunctionName, const QVariantMap& routeData = {}, Qt::ConnectionType connectionType = Qt::AutoConnection);
    void navigationRouteBack();
};

#endif // ELANAVIGATIONROUTER_H
