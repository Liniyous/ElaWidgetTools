#ifndef ELANAVIGATIONROUTERPRIVATE_H
#define ELANAVIGATIONROUTERPRIVATE_H
#include <QObject>
#include <QQueue>
#include <QVariantMap>

#include "ElaProperty.h"
class ElaNavigationRouter;
class ElaNavigationRouterPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaNavigationRouter)
    Q_PROPERTY_CREATE_D(int, MaxRouteCount)
public:
    explicit ElaNavigationRouterPrivate(QObject* parent = nullptr);
    ~ElaNavigationRouterPrivate() override;

private:
    QQueue<QVariantMap> _routeQueue;
};

#endif // ELANAVIGATIONROUTERPRIVATE_H
