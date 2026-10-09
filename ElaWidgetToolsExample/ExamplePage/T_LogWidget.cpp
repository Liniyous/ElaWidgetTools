#include "T_LogWidget.h"

#include <ElaListView.h>

#include <QVBoxLayout>

#include "ElaLog.h"
#include "T_LogModel.h"
T_LogWidget::T_LogWidget(QWidget* parent)
    : QWidget{parent}
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 5, 5, 0);
    ElaListView* logView = new ElaListView(this);
    logView->setIsTransparent(true);
    _logModel = new T_LogModel(this);
    logView->setModel(_logModel);
    mainLayout->addWidget(logView);
    connect(ElaLog::getInstance(), &ElaLog::logMessage, this, [=](QString log) {
        _logModel->appendLogList(log);
    });
    _logModel->appendLogList("[INFO] 日志服务已就绪");
    _logModel->appendLogList("[INFO] 加载界面组件");
    _logModel->appendLogList("[INFO] 初始化导航页面");
    _logModel->appendLogList("[INFO] 等待用户操作");
}

T_LogWidget::~T_LogWidget()
{
}
