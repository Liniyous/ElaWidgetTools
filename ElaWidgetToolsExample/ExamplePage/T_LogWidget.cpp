#include "T_LogWidget.h"

#include "ElaListView.h"
#include "ElaLog.h"
#include "T_LogModel.h"

#include <QSharedPointer>
#include <QTimer>
#include <QVBoxLayout>

T_LogWidget::T_LogWidget(QWidget* parent)
    : QWidget{parent}
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 5, 5, 0);
    ElaListView* logView = new ElaListView(this);
    logView->setIsTransparent(true);
    logView->setUniformItemSizes(true);
    _logModel = new T_LogModel(this);
    logView->setModel(_logModel);
    mainLayout->addWidget(logView);
    QSharedPointer<PendingLogs> pendingLogs(new PendingLogs());
    // 直接回调使用日志单例作为上下文，窗口销毁后在途回调仍可安全结束
    _logConnection = connect(
        ElaLog::getInstance(), &ElaLog::logMessage, ElaLog::getInstance(), [pendingLogs](const QString& log) {
            QMutexLocker locker(&pendingLogs->mutex);
            if (pendingLogs->logs.size() == T_LogModel::MaximumLogCount)
            {
                pendingLogs->logs.removeFirst();
            }
            pendingLogs->logs.append(log);
        },
        Qt::DirectConnection);
    QTimer* refreshTimer = new QTimer(this);
    refreshTimer->setInterval(33);
    connect(refreshTimer, &QTimer::timeout, this, [this, pendingLogs]() {
        QStringList logs;
        {
            QMutexLocker locker(&pendingLogs->mutex);
            logs.swap(pendingLogs->logs);
        }
        _logModel->appendLogs(logs);
    });
    refreshTimer->start();
    // 界面仅保留最近一万条，完整日志由 ElaLog 写入文件
    _logModel->appendLogs({"[信息] 日志服务已就绪", "[信息] 加载界面组件", "[信息] 初始化导航页面", "[信息] 等待用户操作"});
}

T_LogWidget::~T_LogWidget()
{
    disconnect(_logConnection);
}
