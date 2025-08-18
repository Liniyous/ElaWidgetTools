#ifndef ELALOG_H
#define ELALOG_H

#include <QObject>

#include "ElaProperty.h"
#include "ElaSingleton.h"

class ElaLogPrivate;
class ELA_EXPORT ElaLog : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaLog)
    Q_PROPERTY_CREATE_Q_H(QString, LogSavePath)
    Q_PROPERTY_CREATE_Q_H(QString, LogFileName)
    Q_PROPERTY_CREATE_Q_H(bool, IsLogFileNameWithTime)
    Q_SINGLETON_CREATE_H(ElaLog);

private:
    explicit ElaLog(QObject* parent = nullptr);
    ~ElaLog();

public:
    void initMessageLog(bool isEnable);
Q_SIGNALS:
    void logMessage(QString log);
};

#endif // ELALOG_H
