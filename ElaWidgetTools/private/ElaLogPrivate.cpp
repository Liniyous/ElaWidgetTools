#include "ElaLogPrivate.h"

#include "ElaLog.h"
#include "ElaLogWriter.h"

#include <QDateTime>
#include <QDir>
#include <cstdio>

ElaLogPrivate::ElaLogPrivate(QObject* parent)
    : QObject{parent}
{
    _writer = new ElaLogWriter(this);
}

void ElaLogPrivate::_messageLogHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{
    ElaLog* log = ElaLog::getInstance();
    ElaLogPrivate* d = log->d_func();
    if (QThread::currentThread() == d->_writer)
    {
        // 文件系统自身的诊断直接输出，避免写入线程等待自己的队列
        const QByteArray message = msg.toLocal8Bit();
        std::fprintf(stderr, "%s\n", message.constData());
        return;
    }
    const QString logInfo = _formatMessage(type, ctx, msg);
    if (!d->_writer->append(logInfo))
    {
        return;
    }
    // Qt 可将警告和错误升级为进程终止，此时也要在处理器返回前排空
    if (type == QtFatalMsg
        || ((type == QtWarningMsg || type == QtCriticalMsg) && !qgetenv("QT_FATAL_WARNINGS").isEmpty())
        || (type == QtCriticalMsg && !qgetenv("QT_FATAL_CRITICALS").isEmpty()))
    {
        d->_writer->flush();
    }
    Q_EMIT log->logMessage(logInfo);
}

QString ElaLogPrivate::_formatMessage(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{
    // 原有时间格式精确到秒，同一线程在秒内复用格式化结果
    thread_local qint64 cachedSecond = -1;
    thread_local QString cachedTime;
    const qint64 second = QDateTime::currentMSecsSinceEpoch() / 1000;
    if (cachedSecond != second)
    {
        cachedSecond = second;
        cachedTime = QDateTime::fromMSecsSinceEpoch(second * 1000).toString("yyyy-MM-dd hh:mm:ss");
    }
    const char* level = "信息";
    switch (type)
    {
    case QtDebugMsg:
    case QtInfoMsg:
    {
        break;
    }
    case QtWarningMsg:
    {
        level = "警告";
        break;
    }
    case QtCriticalMsg:
    {
        level = "错误";
        break;
    }
    case QtFatalMsg:
    {
        level = "致命";
        break;
    }
    }
    return QString("[%1-%2](函数: %3 , 行数: %4) -> %5")
        .arg(level, cachedTime, QString::fromUtf8(ctx.function ? ctx.function : ""), QString::number(ctx.line), msg);
}

void ElaLogPrivate::_configureLogFile()
{
    QString fileName = _pLogFileName;
    if (_pIsLogFileNameWithTime)
    {
        fileName += QDateTime::currentDateTime().toString("_yyyy-MM-dd_hh-mm-ss");
    }
    _writer->setFilePath(QDir(_pLogSavePath).filePath(fileName + ".txt"), !_pIsLogFileNameWithTime);
}
