#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ElaWindow.h"

#include <QMainWindow>
class T_Home;
class T_Icon;
class T_ElaScreen;
class T_BaseComponents;
class T_Graphics;
class T_Navigation;
class T_Popup;
class T_Card;
class T_ListView;
class T_TableView;
class T_TreeView;
class T_About;
class T_Setting;
class ElaContentDialog;

class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    void initWindow();
    void initEdgeLayout();
    void initContent();

protected:
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    ElaContentDialog* _closeDialog{nullptr};
    ElaSuggestBox* _windowSuggestBox{nullptr};
    T_Home* _homePage{nullptr};
#ifdef Q_OS_WIN
    T_ElaScreen* _elaScreenPage{nullptr};
#endif
    T_Icon* _iconPage{nullptr};
    T_BaseComponents* _baseComponentsPage{nullptr};
    T_Graphics* _graphicsPage{nullptr};
    T_Navigation* _navigationPage{nullptr};
    T_Popup* _popupPage{nullptr};
    T_Card* _cardPage{nullptr};
    T_ListView* _listViewPage{nullptr};
    T_TableView* _tableViewPage{nullptr};
    T_TreeView* _treeViewPage{nullptr};
    T_About* _aboutPage{nullptr};
    T_Setting* _settingPage{nullptr};
    QString _elaDxgiKey{""};
    QString _viewKey{""};
    QString _aboutKey{""};
    QString _settingKey{""};
};
#endif // MAINWINDOW_H
