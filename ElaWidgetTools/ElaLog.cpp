#include "ElaLog.h"

#include "ElaLogPrivate.h"
#include "ElaLogWriter.h"

#include <QCoreApplication>
#include <QDir>

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
    d->_configureLogFile();
    connect(this, &ElaLog::pLogSavePathChanged, d, &ElaLogPrivate::_configureLogFile);
    connect(this, &ElaLog::pLogFileNameChanged, d, &ElaLogPrivate::_configureLogFile);
    connect(this, &ElaLog::pIsLogFileNameWithTimeChanged, d, &ElaLogPrivate::_configureLogFile);
    const auto shutdown = []() {
        ElaLog::getInstance()->initMessageLog(false);
    };
    if (QCoreApplication::instance())
    {
        connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, shutdown, Qt::DirectConnection);
    }
    // 也覆盖未进入事件循环、直接销毁 QCoreApplication 的程序
    qAddPostRoutine(shutdown);
}

ElaLog::~ElaLog()
{
    initMessageLog(false);
}

void ElaLog::initMessageLog(bool isEnable)
{
    Q_D(ElaLog);
    QMutexLocker locker(&d->_controlMutex);
    if (d->_isEnabled == isEnable)
    {
        return;
    }
    if (isEnable)
    {
        d->_writer->startLogging();
        d->_previousHandler = qInstallMessageHandler(ElaLogPrivate::_messageLogHandler);
    }
    else
    {
        const QtMessageHandler currentHandler = qInstallMessageHandler(d->_previousHandler);
        if (currentHandler != ElaLogPrivate::_messageLogHandler)
        {
            qInstallMessageHandler(currentHandler);
        }
        d->_writer->stopLogging();
    }
    d->_isEnabled = isEnable;
}

bool ElaLog::flush()
{
    Q_D(ElaLog);
    return d->_writer->flush();
}
