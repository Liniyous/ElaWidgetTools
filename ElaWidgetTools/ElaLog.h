#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELALOG_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELALOG_H_

#include <QObject>

#include "ElaPropertyMacro.h"
#include "ElaSingletonMacro.h"
#include "ElaWidgetToolsExport.h"

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
    ~ElaLog() override;

public:
    // 启用后异步批量写入；关闭时等待已接收日志写完，并恢复原 Qt 消息处理器
    void initMessageLog(bool isEnable);

    // 等待调用前已接收的日志写入并刷新文件，本次启用期间发生写入失败时返回 false
    bool flush();
Q_SIGNALS:
    // 在产生日志的线程中逐条发出，耗时接收者应自行合并或异步处理
    Q_SIGNAL void logMessage(const QString& log);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELALOG_H_
