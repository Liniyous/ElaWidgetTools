#ifndef T_LOGWIDGET_H
#define T_LOGWIDGET_H

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

#endif // T_LOGWIDGET_H
