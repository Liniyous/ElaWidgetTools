#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAEVENTBUSPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAEVENTBUSPRIVATE_H_

#include <QMap>
#include <QObject>

#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaEvent;
class ElaEventPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaEvent)
    ELA_PROPERTY_D(QString, EventName);
    ELA_PROPERTY_D(QString, FunctionName);
    ELA_PROPERTY_D(Qt::ConnectionType, ConnectionType);

public:
    explicit ElaEventPrivate(QObject* parent = nullptr);
    ~ElaEventPrivate();
};

class ElaEventBus;
class ElaEventBusPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaEventBus)
public:
    explicit ElaEventBusPrivate(QObject* parent = nullptr);
    ~ElaEventBusPrivate();
    ElaEventBusType::EventBusReturnType registerEvent(ElaEvent* event);
    void unRegisterEvent(ElaEvent* event);

private:
    QMap<QString, QList<ElaEvent*>> _eventMap;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAEVENTBUSPRIVATE_H_

