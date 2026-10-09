#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALOGPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALOGPRIVATE_H_

#include <QObject>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaLog;
class ElaLogPrivate : public QObject
{
    Q_OBJECT
    ELA_PROPERTY_D(QString, LogSavePath)
    ELA_PROPERTY_D(QString, LogFileName)
    ELA_PROPERTY_D(bool, IsLogFileNameWithTime)
    ELA_PIMPL_D(ElaLog)
public:
    explicit ElaLogPrivate(QObject* parent = nullptr);
    ~ElaLogPrivate();

private:
    static void _messageLogHander(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
    void _clearLogFile();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALOGPRIVATE_H_

