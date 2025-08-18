#ifndef ELALOGPRIVATE_H
#define ELALOGPRIVATE_H

#include <QObject>

#include "ElaProperty.h"
class ElaLog;
class ElaLogPrivate : public QObject
{
    Q_OBJECT
    Q_PROPERTY_CREATE_D(QString, LogSavePath)
    Q_PROPERTY_CREATE_D(QString, LogFileName)
    Q_PROPERTY_CREATE_D(bool, IsLogFileNameWithTime)
    Q_D_CREATE(ElaLog)
public:
    explicit ElaLogPrivate(QObject* parent = nullptr);
    ~ElaLogPrivate();

private:
    static void _messageLogHander(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
    void _clearLogFile();
};

#endif // ELALOGPRIVATE_H
