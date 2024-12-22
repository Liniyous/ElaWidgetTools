#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QGuiApplication>
#include <QScreen>
#include <QApplication>

#include "signal.h"
#include "Def.h"
#include "ElaLog.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaWindow.h"
#include "ElaApplication.h"
#include "ElaContentDialog.h"
#include "ElaScrollPage.h"
#include "ElaLoadingDialog.h"
#include "ExamplePage/T_Home.h"
#include "ExamplePage/T_BaseComponents.h"
#include "ExamplePage/T_Setting.h"

#include "Interface/c_timepage.h"
#include "Interface/loginwindow.h"
#include "Function/database.h"

class T_Home;
class T_BaseComponents;
class ElaContentDialog;
class ElaLoadingDialog;
class T_Setting;
class TimeForm;
class MainWindow : public ElaWindow
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(MainWindow)
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showLoading(bool loading, int ms=5000);

private:
    void initWindow();
    void initContent();
    void initDialog();

    T_Home* _homePage{nullptr};
    T_BaseComponents* _baseComponentsPage{nullptr};
    T_Setting* _settingPage{nullptr};

    ElaContentDialog* _closeDialog{nullptr};
    ElaLoadingDialog* _loadingDialog{nullptr};

    QString _elaDxgiKey{""};
    QString _viewKey{""};
    QString _aboutKey{""};
    QString _settingKey{""};
};

#endif // MAINWINDOW_H
