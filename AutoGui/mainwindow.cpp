#include "mainwindow.h"

#include <QDebug>
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
#include "ElaProgressBar.h"
#include "ElaStatusBar.h"
#include "ElaText.h"
#include "ElaToolBar.h"
#include "ElaToolButton.h"
#include "T_About.h"
#include "T_BaseComponents.h"
#include "T_Card.h"
#include "T_View.h"
#ifdef Q_OS_WIN
#include "ExamplePage/T_ElaScreen.h"
#endif
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
    ElaContentDialog dialog(this);
    connect(&dialog, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    connect(&dialog, &ElaContentDialog::middleButtonClicked, this, &MainWindow::showMinimized);
    dialog.exec();
}

void MainWindow::initWindow()
{
    setWindowIcon(QIcon(":/include/Image/Cirno.jpg"));
    resize(1200, 740);
    // ElaLog::getInstance()->initMessageLog(true);
    // eApp->setThemeMode(ElaThemeType::Dark);
    // setIsNavigationBarEnable(false);
    // setNavigationBarDisplayMode(ElaNavigationType::Compact);
    // setWindowButtonFlag(ElaAppBarType::MinimizeButtonHint, false);
    setUserInfoCardPixmap(QPixmap(":/Resource/Image/Cirno.jpg"));
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
    menuBar->setFixedHeight(30);
    QWidget* customWidget = new QWidget(this);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(0, 0, 0, 0);
    customLayout->addWidget(menuBar);
    customLayout->addStretch();
    //this->setMenuBar(menuBar);
    this->setCustomWidget(customWidget);
    this->setCustomWidgetMaximumWidth(500);

    menuBar->addElaIconAction(ElaIconType::AtomSimple, "动作菜单");
    ElaMenu* iconMenu = menuBar->addMenu(ElaIconType::Aperture, "图标菜单");
    iconMenu->setMenuItemHeight(27);
    iconMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::SelectAll);
    iconMenu->addElaIconAction(ElaIconType::Copy, "复制");
    iconMenu->addElaIconAction(ElaIconType::MagnifyingGlassPlus, "显示设置");
    iconMenu->addSeparator();
    iconMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
    iconMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
    menuBar->addSeparator();
    ElaMenu* shortCutMenu = new ElaMenu("快捷菜单(&A)", this);
    shortCutMenu->setMenuItemHeight(27);
    shortCutMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::Find);
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
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    toolBar->setToolBarSpacing(3);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBar->setIconSize(QSize(25, 25));
    // toolBar->setFloatable(false);
    // toolBar->setMovable(false);
    ElaToolButton* toolButton1 = new ElaToolButton(this);
    toolButton1->setElaIcon(ElaIconType::BadgeCheck);
    toolBar->addWidget(toolButton1);
    ElaToolButton* toolButton2 = new ElaToolButton(this);
    toolButton2->setElaIcon(ElaIconType::ChartUser);
    toolBar->addWidget(toolButton2);
    toolBar->addSeparator();
    ElaToolButton* toolButton3 = new ElaToolButton(this);
    toolButton3->setElaIcon(ElaIconType::Bluetooth);
    toolButton3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolButton3->setText("Bluetooth");
    toolBar->addWidget(toolButton3);
    ElaToolButton* toolButton4 = new ElaToolButton(this);
    toolButton4->setElaIcon(ElaIconType::BringFront);
    toolBar->addWidget(toolButton4);
    toolBar->addSeparator();
    ElaToolButton* toolButton5 = new ElaToolButton(this);
    toolButton5->setElaIcon(ElaIconType::ChartSimple);
    toolBar->addWidget(toolButton5);
    ElaToolButton* toolButton6 = new ElaToolButton(this);
    toolButton6->setElaIcon(ElaIconType::FaceClouds);
    toolBar->addWidget(toolButton6);
    ElaToolButton* toolButton8 = new ElaToolButton(this);
    toolButton8->setElaIcon(ElaIconType::Aperture);
    toolBar->addWidget(toolButton8);
    ElaToolButton* toolButton9 = new ElaToolButton(this);
    toolButton9->setElaIcon(ElaIconType::ChartMixed);
    toolBar->addWidget(toolButton9);
    ElaToolButton* toolButton10 = new ElaToolButton(this);
    toolButton10->setElaIcon(ElaIconType::Coins);
    toolBar->addWidget(toolButton10);
    ElaToolButton* toolButton11 = new ElaToolButton(this);
    toolButton11->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolButton11->setElaIcon(ElaIconType::AlarmPlus);
    toolButton11->setText("AlarmPlus");
    toolBar->addWidget(toolButton11);
    ElaToolButton* toolButton12 = new ElaToolButton(this);
    toolButton12->setElaIcon(ElaIconType::Crown);
    toolBar->addWidget(toolButton12);
    QAction* test = new QAction(this);
    test->setMenu(new QMenu(this));

    ElaProgressBar* progressBar = new ElaProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    progressBar->setFixedWidth(350);
    toolBar->addWidget(progressBar);

    this->addToolBar(Qt::TopToolBarArea, toolBar);

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
#ifdef Q_OS_WIN
    _elaScreenPage = new T_ElaScreen(this);
#endif
    _iconPage = new T_Icon(this);
    _baseComponentsPage = new T_BaseComponents(this);
    _tabWidgetPage = new T_TabWidget(this);
    _popupPage = new T_Popup(this);
    _cardPage = new T_Card(this);
    _viewPage = new T_View(this);

    // GraphicsView
    ElaGraphicsScene* scene = new ElaGraphicsScene(this);
    scene->setSceneRect(0, 0, 1500, 1500);
    ElaGraphicsItem* item1 = new ElaGraphicsItem();
    item1->setWidth(100);
    item1->setHeight(100);
    item1->setMaxLinkPortCount(100);
    item1->setMaxLinkPortCount(1);
    ElaGraphicsItem* item2 = new ElaGraphicsItem();
    item2->setWidth(100);
    item2->setHeight(100);
    scene->addItem(item1);
    scene->addItem(item2);
    ElaGraphicsView* view = new ElaGraphicsView(scene);
    view->setScene(scene);

    QString testKey_1;
    QString testKey_2;
    addPageNode("HOME", _homePage, ElaIconType::House);
#ifdef Q_OS_WIN
    addExpanderNode("ElaDxgi", _elaDxgiKey, ElaIconType::TvMusic);
    addPageNode("ElaScreen", _elaScreenPage, _elaDxgiKey, 3, ElaIconType::ObjectGroup);
#endif
    // navigation(elaScreenWidget->property("ElaPageKey").toString());
    addPageNode("ElaBaseComponents", _baseComponentsPage, ElaIconType::CabinetFiling);
    addPageNode("ElaView", _viewPage, ElaIconType::CameraViewfinder);
    addPageNode("ElaGraphics", view, 9, ElaIconType::KeySkeleton);
    addPageNode("ElaCard", _cardPage, ElaIconType::Cards);
    addPageNode("ElaTabWidget", _tabWidgetPage, ElaIconType::Table);
    addPageNode("ElaPopup", _popupPage, ElaIconType::Envelope);
    addPageNode("ElaIcon", _iconPage, 99, ElaIconType::FontAwesome);
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
    T_About* aboutPage = new T_About();
    aboutPage->hide();
    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey) {
        if (_aboutKey == nodeKey)
        {
            aboutPage->setFixedSize(400, 400);
            aboutPage->moveToCenter();
            aboutPage->show();
        }
    });
    addFooterNode("Setting", new QWidget(this), _settingKey, 0, ElaIconType::GearComplex);
    connect(this, &MainWindow::userInfoCardClicked, this, [=]() { this->navigation(_homePage->property("ElaPageKey").toString()); });
#ifdef Q_OS_WIN
    connect(_homePage, &T_Home::elaScreenNavigation, this, [=]() { this->navigation(_elaScreenPage->property("ElaPageKey").toString()); });
#endif
    connect(_homePage, &T_Home::elaBaseComponentNavigation, this, [=]() { this->navigation(_baseComponentsPage->property("ElaPageKey").toString()); });
    connect(_homePage, &T_Home::elaSceneNavigation, this, [=]() { this->navigation(view->property("ElaPageKey").toString()); });
    connect(_homePage, &T_Home::elaIconNavigation, this, [=]() { this->navigation(_iconPage->property("ElaPageKey").toString()); });
    connect(_homePage, &T_Home::elaCardNavigation, this, [=]() { this->navigation(_cardPage->property("ElaPageKey").toString()); });
    qDebug() << "已注册的事件列表" << ElaEventBus::getInstance()->getRegisteredEventsName();
}
