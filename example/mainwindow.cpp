#include "mainwindow.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QStatusBar>
#include <QVBoxLayout>

#include "ElaContentDialog.h"
#include "ElaDockWidget.h"
#include "ElaEventBus.h"
#include "ElaGraphicsItem.h"
#include "ElaGraphicsScene.h"
#include "ElaGraphicsView.h"
#include "ElaLog.h"
#include "ElaMenu.h"
#include "ElaMenuBar.h"
#include "ElaStatusBar.h"
#include "ElaText.h"
#include "ElaToolBar.h"
#include "ElaToolButton.h"
#include "ElaWidget.h"
#include "ExamplePage/T_BaseComponents.h"
#include "ExamplePage/T_ElaScreen.h"
#include "ExamplePage/T_Home.h"
#include "ExamplePage/T_Icon.h"
#include "ExamplePage/T_LogWidget.h"
#include "ExamplePage/T_Popup.h"
#include "ExamplePage/T_TabWidget.h"
#include "ExamplePage/T_UpdateWidget.h"
MainWindow::MainWindow(QWidget* parent)
    : ElaWindow(parent)
{
    initWindow();

    //额外布局
    initEdgeLayout();

    //中心窗口
    initContent();

    // 拦截默认关闭事件
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this, &MainWindow::onCloseButtonClicked);

    //移动到中心
    moveToCenter();
}

MainWindow::~MainWindow()
{
}

void MainWindow::onCloseButtonClicked()
{
    ElaContentDialog* dialag = new ElaContentDialog(this);
    connect(dialag, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    connect(dialag, &ElaContentDialog::middleButtonClicked, this, &MainWindow::showMinimized);
    dialag->show();
}

void MainWindow::initWindow()
{
    resize(1240, 740);
    // ElaLog::getInstance()->initMessageLog(true);
    // eApp->setThemeMode(ElaThemeType::Dark);
    // setIsNavigationBarEnable(false);
    // setNavigationBarDisplayMode(ElaNavigationType::Compact);
    // setWindowButtonFlag(ElaAppBarType::MinimizeButtonHint, false);
    setUserInfoCardPixmap(QPixmap(":/include/Image/Cirno.jpg"));
    setUserInfoCardTitle("Ela Tool");
    setUserInfoCardSubTitle("Liniyous@gmail.com");
    setWindowTitle("ElaWidgetTool");
    // setIsStayTop(true);
    // setUserInfoCardVisible(false);
}

void MainWindow::initEdgeLayout()
{
    //菜单栏
    ElaMenuBar* menuBar = new ElaMenuBar(this);
    this->setMenuBar(menuBar);

    ElaMenu* iconMenu = menuBar->addMenu(ElaIconType::DeerRudolph, "图标菜单");
    iconMenu->setMenuItemHeight(27);
    iconMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::Save);
    iconMenu->addElaIconAction(ElaIconType::Copy, "复制");
    iconMenu->addElaIconAction(ElaIconType::MagnifyingGlassPlus, "显示设置");
    iconMenu->addSeparator();
    iconMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
    iconMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");

    menuBar->addAction("动作菜单");
    menuBar->addSeparator();
    ElaMenu* shortCutMenu = new ElaMenu("快捷菜单(&A)", this);
    shortCutMenu->setMenuItemHeight(27);
    shortCutMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::Save);
    shortCutMenu->addElaIconAction(ElaIconType::Copy, "复制");
    shortCutMenu->addElaIconAction(ElaIconType::MagnifyingGlassPlus, "显示设置");
    shortCutMenu->addSeparator();
    shortCutMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
    shortCutMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
    menuBar->addMenu(shortCutMenu);

    menuBar->addMenu("样例菜单(&B)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&C)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&D)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&E)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&F)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&G)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");

    //工具栏
    ElaToolBar* toolBar = new ElaToolBar("工具栏", this);
    ElaToolButton* toolButton1 = new ElaToolButton(this);
    toolButton1->setElaIcon(ElaIconType::BadgeCheck);
    toolBar->addWidget(toolButton1);
    ElaToolButton* toolButton2 = new ElaToolButton(this);
    toolButton2->setElaIcon(ElaIconType::BaseballBatBall);
    toolBar->addWidget(toolButton2);
    toolBar->addSeparator();
    ElaToolButton* toolButton3 = new ElaToolButton(this);
    toolButton3->setElaIcon(ElaIconType::Bluetooth);
    toolBar->addWidget(toolButton3);
    ElaToolButton* toolButton4 = new ElaToolButton(this);
    toolButton4->setElaIcon(ElaIconType::GameBoard);
    toolBar->addWidget(toolButton4);
    toolBar->addSeparator();
    ElaToolButton* toolButton5 = new ElaToolButton(this);
    toolButton5->setElaIcon(ElaIconType::CircleExclamationCheck);
    toolBar->addWidget(toolButton5);
    ElaToolButton* toolButton6 = new ElaToolButton(this);
    toolButton6->setElaIcon(ElaIconType::FaceClouds);
    toolBar->addWidget(toolButton6);
    ElaToolButton* toolButton7 = new ElaToolButton(this);
    toolButton7->setElaIcon(ElaIconType::SquareThisWayUp);
    toolBar->addWidget(toolButton7);
    this->addToolBar(Qt::LeftToolBarArea, toolBar);

    //停靠窗口
    ElaDockWidget* logDockWidget = new ElaDockWidget("日志信息", this);
    logDockWidget->setWidget(new T_LogWidget(this));
    this->addDockWidget(Qt::RightDockWidgetArea, logDockWidget);
    resizeDocks({logDockWidget}, {200}, Qt::Horizontal);

    ElaDockWidget* updateDockWidget = new ElaDockWidget("更新内容", this);
    updateDockWidget->setWidget(new T_UpdateWidget(this));
    this->addDockWidget(Qt::RightDockWidgetArea, updateDockWidget);
    resizeDocks({updateDockWidget}, {200}, Qt::Horizontal);

    //状态栏
    ElaStatusBar* statusBar = new ElaStatusBar(this);
    ElaText* statusText = new ElaText("初始化成功！", this);
    statusText->setTextPixelSize(14);
    statusBar->addWidget(statusText);
    this->setStatusBar(statusBar);
}

void MainWindow::initContent()
{
    _homePage = new T_Home(this);
    _elaScreenPage = new T_ElaScreen(this);
    _iconPage = new T_Icon(this);
    _baseComponentsPage = new T_BaseComponents(this);
    _tabWidgetPage = new T_TabWidget(this);
    _popupPage = new T_Popup(this);

    // GraphicsView
    ElaGraphicsScene* scene = new ElaGraphicsScene(this);
    scene->setSceneRect(0, 0, 1500, 1500);
    // scene->setSceneRect(0, 0, 1000, 1000);
    ElaGraphicsItem* item1 = new ElaGraphicsItem();
    item1->setWidth(100);
    item1->setHeight(100);
    ElaGraphicsItem* item2 = new ElaGraphicsItem();
    item2->setWidth(100);
    item2->setHeight(100);
    // ElaGraphicsItem* item3 = new ElaGraphicsItem();
    // item3->setWidth(100);
    // item3->setHeight(100);
    // item3->setPos(10, 10);
    scene->addItem(item1);
    scene->addItem(item2);
    // scene->addItem(item3);
    ElaGraphicsView* view = new ElaGraphicsView(scene);
    view->setScene(scene);

    QString testKey_1;
    QString testKey_2;
    addPageNode("HOME", _homePage, ElaIconType::House);
    addExpanderNode("ElaDxgi", _elaDxgiKey, ElaIconType::TvMusic);
    addPageNode("ElaScreen", _elaScreenPage, _elaDxgiKey, 3, ElaIconType::ObjectGroup);
    // navigation(elaScreenWidget->property("ElaPageKey").toString());
    addPageNode("ElaBaseComponents", _baseComponentsPage, ElaIconType::CabinetFiling);
    addPageNode("ElaGraphics", view, 9, ElaIconType::KeySkeleton);
    addPageNode("ElaIcon", _iconPage, 99, ElaIconType::FontAwesome);
    addPageNode("ElaTabWidget", _tabWidgetPage, ElaIconType::Table);
    addPageNode("ElaPopup", _popupPage, ElaIconType::Envelope);
    addExpanderNode("TEST4", testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST5", testKey_1, testKey_2, ElaIconType::Acorn);
    addPageNode("Third Level", new QWidget(this), testKey_1, ElaIconType::Acorn);
    addExpanderNode("TEST6", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST7", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST8", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST9", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST10", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST11", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST12", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST13", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST14", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST15", testKey_1, ElaIconType::Acorn);
    addExpanderNode("TEST16", testKey_1, ElaIconType::Acorn);
    addExpanderNode("TEST17", testKey_1, ElaIconType::Acorn);

    addFooterNode("About", nullptr, _aboutKey, 0, ElaIconType::User);
    ElaWidget* widget = new ElaWidget();
    widget->setWindowModality(Qt::ApplicationModal);
    widget->hide();
    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey) {
        if (_aboutKey == nodeKey)
        {
            widget->show();
        }
    });
    addFooterNode("Setting", new QWidget(this), _settingKey, 0, ElaIconType::GearComplex);
    connect(this, &MainWindow::userInfoCardClicked, this, [=]() { this->navigation(_homePage->property("ElaPageKey").toString()); });
    connect(_homePage, &T_Home::elaScreenNavigation, this, [=]() { this->navigation(_elaScreenPage->property("ElaPageKey").toString()); });
    connect(_homePage, &T_Home::elaBaseComponentNavigation, this, [=]() { this->navigation(_baseComponentsPage->property("ElaPageKey").toString()); });
    connect(_homePage, &T_Home::elaSceneNavigation, this, [=]() { this->navigation(view->property("ElaPageKey").toString()); });
    connect(_homePage, &T_Home::elaIconNavigation, this, [=]() { this->navigation(_iconPage->property("ElaPageKey").toString()); });
    qDebug() << "已注册的事件列表" << ElaEventBus::getInstance()->getRegisteredEventsName();
}
