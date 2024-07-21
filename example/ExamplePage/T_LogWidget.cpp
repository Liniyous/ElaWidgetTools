#include "T_LogWidget.h"

#include <ElaListView.h>

#include <QVBoxLayout>

#include "ElaLog.h"
#include "T_LogModel.h"
T_LogWidget::T_LogWidget(QWidget* parent)
    : QWidget{parent}
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    ElaListView* logView = new ElaListView(this);
    _logModel = new T_LogModel(this);
    logView->setModel(_logModel);
    mainLayout->addWidget(logView);
    connect(ElaLog::getInstance(), &ElaLog::logMessage, this, [=](QString log) {
        _logModel->appendLogList(log);
    });
}

T_LogWidget::~T_LogWidget()
{
}
