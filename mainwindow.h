#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ElaWindow.h"
class T_Home;
class T_Icon;
class T_ElaScreen;
class T_BaseComponents;
class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    Q_SLOT void onCloseButtonClicked();

private:
    T_Home* _homePage{nullptr};
    T_ElaScreen* _elaScreenPage{nullptr};
    T_Icon* _iconPage{nullptr};
    T_BaseComponents* _baseComponentsPage{nullptr};
};
#endif // MAINWINDOW_H
