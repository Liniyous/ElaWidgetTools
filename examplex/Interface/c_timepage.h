#ifndef C_TIMEPAGE_H
#define C_TIMEPAGE_H

#include <QWidget>
#include <QAbstractItemView>
#include "singleton.h"
#include "../mainwindow.h"

namespace Ui {
class C_TimePage;
}

class C_TimePage : public QWidget
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(C_TimePage)
public:
    explicit C_TimePage(QWidget *parent = nullptr);
    ~C_TimePage();

private slots:
    void on_pushButton_clicked();

private:
    Ui::C_TimePage *ui;

    void initUI();
};

#endif // C_TIMEPAGE_H
