#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_LOGWIDGET_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_LOGWIDGET_H_

#include <QMutex>
#include <QStringList>
#include <QWidget>

class T_LogModel;
class T_LogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit T_LogWidget(QWidget* parent = nullptr);
    ~T_LogWidget();

private:
    struct PendingLogs {
        QMutex mutex;     // 保护日志线程写入与界面线程取出的缓冲
        QStringList logs; // 等待下一次界面刷新的最近日志
    };

    T_LogModel* _logModel{nullptr};
    QMetaObject::Connection _logConnection;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_LOGWIDGET_H_
