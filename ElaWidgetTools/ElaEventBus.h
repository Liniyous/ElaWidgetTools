#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAEVENTBUS_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAEVENTBUS_H_

#include <QObject>
#include <QVariantMap>

#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
#include "ElaSingletonMacro.h"
class ElaEventPrivate;
class ELA_EXPORT ElaEvent : public QObject
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaEvent)
    ELA_PROPERTY_Q_H(QString, EventName);
    ELA_PROPERTY_Q_H(QString, FunctionName);
    ELA_PROPERTY_Q_H(Qt::ConnectionType, ConnectionType);

public:
    explicit ElaEvent(QObject* parent = nullptr);
    explicit ElaEvent(const QString& eventName, const QString& functionName, QObject* parent = nullptr);
    ~ElaEvent() override;
    ElaEventBusType::EventBusReturnType registerAndInit();
};

class ElaEventBusPrivate;
class ELA_EXPORT ElaEventBus : public QObject
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaEventBus)
    ELA_SINGLETON_H(ElaEventBus);

private:
    explicit ElaEventBus(QObject* parent = nullptr);
    ~ElaEventBus() override;

public:
    ElaEventBusType::EventBusReturnType post(const QString& eventName, const QVariantMap& data = {});
    QStringList getRegisteredEventsName() const;

private:
    friend class ElaEvent;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAEVENTBUS_H_
