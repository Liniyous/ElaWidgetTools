#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELALOG_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELALOG_H_

#include <QObject>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
#include "ElaSingletonMacro.h"

class ElaLogPrivate;
class ELA_EXPORT ElaLog : public QObject
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaLog)
    ELA_PROPERTY_Q_H(QString, LogSavePath)
    ELA_PROPERTY_Q_H(QString, LogFileName)
    ELA_PROPERTY_Q_H(bool, IsLogFileNameWithTime)
    ELA_SINGLETON_H(ElaLog);

private:
    explicit ElaLog(QObject* parent = nullptr);
    ~ElaLog();

public:
    void initMessageLog(bool isEnable);
Q_SIGNALS:
    void logMessage(const QString& log);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELALOG_H_
