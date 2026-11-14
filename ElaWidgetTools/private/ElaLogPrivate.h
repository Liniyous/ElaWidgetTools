#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALOGPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALOGPRIVATE_H_

#include "ElaPropertyMacro.h"

#include <QMutex>
#include <QObject>

class ElaLog;
class ElaLogWriter;
class ElaLogPrivate : public QObject
{
    Q_OBJECT
    ELA_PROPERTY_D(QString, LogSavePath)
    ELA_PROPERTY_D(QString, LogFileName)
    ELA_PROPERTY_D(bool, IsLogFileNameWithTime)
    ELA_PIMPL_D(ElaLog)
public:
    explicit ElaLogPrivate(QObject* parent = nullptr);

private:
    static void _messageLogHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
    static QString _formatMessage(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
    void _configureLogFile();

    ElaLogWriter* _writer{nullptr};
    QMutex _controlMutex;
    QtMessageHandler _previousHandler{nullptr};
    bool _isEnabled{false};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALOGPRIVATE_H_
