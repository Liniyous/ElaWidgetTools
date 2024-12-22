#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QRect>

#include "singleton.h"
#include "ElaWidget.h"
#include "ElaMessageBar.h"
#include "../Function/database.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public ElaWidget
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(LoginWindow)

public:
    ~LoginWindow();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private slots:
    void on_btnLogin_clicked();

private:
    explicit LoginWindow(QWidget *parent = nullptr);
    Ui::LoginWindow *ui;

    bool _isEnableMica;
    ElaThemeType::ThemeMode _themeMode;

    QPixmap _pixmap;
};

#endif // LOGINWINDOW_H
