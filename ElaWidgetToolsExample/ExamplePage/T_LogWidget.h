#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_LOGWIDGET_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_LOGWIDGET_H_

#include <QWidget>

class T_LogModel;
class T_LogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit T_LogWidget(QWidget* parent = nullptr);
    ~T_LogWidget();
signals:
private:
    T_LogModel* _logModel{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_LOGWIDGET_H_

