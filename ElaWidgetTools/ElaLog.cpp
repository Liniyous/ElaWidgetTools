#include "ElaLog.h"

#include <QDir>

#include "ElaLogPrivate.h"

ELA_SINGLETON_CPP(ElaLog)
ELA_PROPERTY_Q_CPP(ElaLog, QString, LogSavePath)
ELA_PROPERTY_Q_CPP(ElaLog, QString, LogFileName)
ELA_PROPERTY_Q_CPP(ElaLog, bool, IsLogFileNameWithTime)
ElaLog::ElaLog(QObject* parent)
    : QObject{parent}, d_ptr(new ElaLogPrivate())
{
    Q_D(ElaLog);
    d->q_ptr = this;
    d->_pLogFileName = "ElaLog";
    d->_pLogSavePath = QDir::currentPath();
    d->_pIsLogFileNameWithTime = false;
    d->_clearLogFile();
    connect(this, &ElaLog::pLogSavePathChanged, d, &ElaLogPrivate::_clearLogFile);
    connect(this, &ElaLog::pLogFileNameChanged, d, &ElaLogPrivate::_clearLogFile);
    connect(this, &ElaLog::pIsLogFileNameWithTimeChanged, d, &ElaLogPrivate::_clearLogFile);
}

ElaLog::~ElaLog()
{
}

void ElaLog::initMessageLog(bool isEnable)
{
    Q_D(ElaLog);
    qInstallMessageHandler(isEnable ? d->_messageLogHander : 0);
}
