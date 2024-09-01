#ifndef T_mainwindow_H
#define T_mainwindow_H

#include <QMainWindow>
#include "DxgiWidget.h"
#include "ElaWindow.h"
#include "ElaDockWidget.h"
class T_Home;
class T_Icon;
class T_ElaScreen;
class T_BaseComponents;
class T_TabWidget;
class T_Popup;
class T_Card;
class T_View;
class T_mainwindow : public ElaWindow
{
    Q_OBJECT

public:
    T_mainwindow(QWidget* parent = nullptr);
    ~T_mainwindow();
    Q_SLOT void onCloseButtonClicked();

    void initWindow();
    void initEdgeLayout();
    void initContent();

private:
    T_Home* _homePage{nullptr};
    DxgiWidget * dxgiWidget{nullptr};
    ElaDockWidget* paddleDockWidget{nullptr};
    ElaDockWidget* updateDockWidget{nullptr};
    ElaDockWidget* logDockWidget{nullptr};
#ifdef Q_OS_WIN
    T_ElaScreen* _elaScreenPage{nullptr};
#endif
    T_Icon* _iconPage{nullptr};
    T_BaseComponents* _baseComponentsPage{nullptr};
    T_TabWidget* _tabWidgetPage{nullptr};
    T_Popup* _popupPage{nullptr};
    T_Card* _cardPage{nullptr};
    T_View* _viewPage{nullptr};
    QString _elaDxgiKey{""};
    QString _aboutKey{""};
    QString _settingKey{""};
    
};
#endif // T_mainwindow_H
