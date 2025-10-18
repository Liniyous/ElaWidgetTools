#include "mainwindow.h"

#include "ElaContentDialog.h"
#include "ElaDockWidget.h"
#include "ElaEventBus.h"
#include "ElaLog.h"
#include "ElaMenu.h"
#include "ElaMenuBar.h"
#include "ElaNavigationRouter.h"
#include "ElaProgressBar.h"
#include "ElaProgressRing.h"
#include "ElaStatusBar.h"
#include "ElaSuggestBox.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolBar.h"
#include "ElaToolButton.h"
#include "T_About.h"
#include "T_BaseComponents.h"
#include "T_Card.h"
#include "T_Graphics.h"
#include "T_ListView.h"
#include "T_Setting.h"
#include "T_TableView.h"
#include "T_TreeView.h"
#include <QDebug>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QMouseEvent>
#ifdef Q_OS_WIN
#include "ElaApplication.h"
#include "ExamplePage/T_ElaScreen.h"
#include <QTimer>
#endif

#include "ExamplePage/T_Home.h"
#include "ExamplePage/T_Icon.h"
#include "ExamplePage/T_LogWidget.h"
#include "ExamplePage/T_Navigation.h"
#include "ExamplePage/T_Popup.h"
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
    _closeDialog = new ElaContentDialog(this);
    connect(_closeDialog, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    connect(_closeDialog, &ElaContentDialog::middleButtonClicked, this, [=]() {
        _closeDialog->close();
        showMinimized();
    });
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this, [=]() {
        _closeDialog->exec();
    });

    //移动到中心
    //moveToCenter();

    //  如果你的windows版本低于Win11 调用原生Mica、Mica-Alt、Acrylic 会导致窗口绘制失效  Dwm_Blur仍可使用
    //    eTheme->setThemeMode(ElaThemeType::Dark);
    //    QTimer::singleShot(1, this, [=]() {
    //        eApp->setWindowDisplayMode(ElaApplicationType::Mica);
    //    });

    //    QTimer::singleShot(1, this, [=]() {
    //        showFullScreen();
    //    });
}

MainWindow::~MainWindow()
{
    delete this->_aboutPage;
}

void MainWindow::initWindow()
{
    setFocusPolicy(Qt::StrongFocus);
    // setIsCentralStackedWidgetTransparent(true);
    setWindowIcon(QIcon(":/include/Image/Cirno.jpg"));
    resize(1200, 740);
    // eTheme->setThemeMode(ElaThemeType::Dark);
    // setIsNavigationBarEnable(false);
    // setNavigationBarDisplayMode(ElaNavigationType::Compact);
    // setWindowButtonFlag(ElaAppBarType::MinimizeButtonHint, false);
    setUserInfoCardPixmap(QPixmap(":/Resource/Image/Cirno.jpg"));
    setUserInfoCardTitle("Ela Tool");
    setUserInfoCardSubTitle("Liniyous@gmail.com");
    setWindowTitle("ElaWidgetTool");
    // setIsStayTop(true);
    // setUserInfoCardVisible(false);
    // setNavigationBarWidth(260);
    ElaText* centralStack = new ElaText("这是一个主窗口堆栈页面", this);
    centralStack->setFocusPolicy(Qt::StrongFocus);
    centralStack->setTextPixelSize(32);
    centralStack->setAlignment(Qt::AlignCenter);
    addCentralWidget(centralStack);

    // 自定义AppBar菜单
    ElaMenu* appBarMenu = new ElaMenu(this);
    appBarMenu->setMenuItemHeight(27);
    connect(appBarMenu->addAction("跳转到一级主要堆栈"), &QAction::triggered, this, [=]() {
        setCurrentStackIndex(0);
    });
    connect(appBarMenu->addAction("跳转到二级主要堆栈"), &QAction::triggered, this, [=]() {
        setCurrentStackIndex(1);
    });
    connect(appBarMenu->addAction("更改页面切换特效(Scale)"), &QAction::triggered, this, [=]() {
        setStackSwitchMode(ElaWindowType::StackSwitchMode::Scale);
    });
    connect(appBarMenu->addElaIconAction(ElaIconType::GearComplex, "自定义主窗口设置"), &QAction::triggered, this, [=]() {
        navigation(_settingKey);
    });
    appBarMenu->addSeparator();
    connect(appBarMenu->addElaIconAction(ElaIconType::MoonStars, "更改项目主题"), &QAction::triggered, this, [=]() {
        eTheme->setThemeMode(eTheme->getThemeMode() == ElaThemeType::Light ? ElaThemeType::Dark : ElaThemeType::Light);
    });
    connect(appBarMenu->addAction("使用原生菜单"), &QAction::triggered, this, [=]() {
        setCustomMenu(nullptr);
    });
    setCustomMenu(appBarMenu);

    // 堆栈独立自定义窗口
    QWidget* centralCustomWidget = new QWidget(this);
    QHBoxLayout* centralCustomWidgetLayout = new QHBoxLayout(centralCustomWidget);
    centralCustomWidgetLayout->setContentsMargins(13, 15, 9, 6);
    ElaToolButton* leftButton = new ElaToolButton(this);
    leftButton->setElaIcon(ElaIconType::AngleLeft);
    leftButton->setEnabled(false);
    connect(leftButton, &ElaToolButton::clicked, this, [=]() {
        ElaNavigationRouter::getInstance()->navigationRouteBack();
    });
    ElaToolButton* rightButton = new ElaToolButton(this);
    rightButton->setElaIcon(ElaIconType::AngleRight);
    rightButton->setEnabled(false);
    connect(rightButton, &ElaToolButton::clicked, this, [=]() {
        ElaNavigationRouter::getInstance()->navigationRouteForward();
    });
    connect(ElaNavigationRouter::getInstance(), &ElaNavigationRouter::navigationRouterStateChanged, this, [=](ElaNavigationRouterType::RouteMode routeMode) {
        switch (routeMode)
        {
        case ElaNavigationRouterType::BackValid:
        {
            leftButton->setEnabled(true);
            break;
        }
        case ElaNavigationRouterType::BackInvalid:
        {
            leftButton->setEnabled(false);
            break;
        }
        case ElaNavigationRouterType::ForwardValid:
        {
            rightButton->setEnabled(true);
            break;
        }
        case ElaNavigationRouterType::ForwardInvalid:
        {
            rightButton->setEnabled(false);
            break;
        }
        }
    });
    ElaSuggestBox* centralStackSuggest = new ElaSuggestBox(this);
    centralStackSuggest->setFixedHeight(32);
    centralStackSuggest->setPlaceholderText("搜索关键字");

    ElaText* progressBusyRingText = new ElaText("系统运行中", this);
    progressBusyRingText->setTextPixelSize(15);

    ElaProgressRing* progressBusyRing = new ElaProgressRing(this);
    progressBusyRing->setBusyingWidth(4);
    progressBusyRing->setFixedSize(28, 28);
    progressBusyRing->setIsBusying(true);

    centralCustomWidgetLayout->addWidget(leftButton);
    centralCustomWidgetLayout->addWidget(rightButton);
    centralCustomWidgetLayout->addWidget(centralStackSuggest);
    centralCustomWidgetLayout->addStretch();
    centralCustomWidgetLayout->addWidget(progressBusyRingText);
    centralCustomWidgetLayout->addWidget(progressBusyRing);
    setCentralCustomWidget(centralCustomWidget);
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
    // this->setMenuBar(menuBar);
    this->setCustomWidget(ElaAppBarType::MiddleArea, customWidget);
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
    _graphicsPage = new T_Graphics(this);
    _navigationPage = new T_Navigation(this);
    _popupPage = new T_Popup(this);
    _cardPage = new T_Card(this);
    _listViewPage = new T_ListView(this);
    _tableViewPage = new T_TableView(this);
    _treeViewPage = new T_TreeView(this);
    _settingPage = new T_Setting(this);

    QString testKey_1;
    QString testKey_2;
    addPageNode("HOME", _homePage, ElaIconType::House);
#ifdef Q_OS_WIN
    addExpanderNode("ElaDxgi", _elaDxgiKey, ElaIconType::TvMusic);
    addPageNode("ElaScreen", _elaScreenPage, _elaDxgiKey, 3, ElaIconType::ObjectGroup);
#endif
    // navigation(elaScreenWidget->property("ElaPageKey").toString());
    addPageNode("ElaBaseComponents", _baseComponentsPage, ElaIconType::CabinetFiling);

    addExpanderNode("ElaView", _viewKey, ElaIconType::CameraViewfinder);
    addPageNode("ElaListView", _listViewPage, _viewKey, 9, ElaIconType::List);
    addPageNode("ElaTableView", _tableViewPage, _viewKey, ElaIconType::Table);
    addPageNode("ElaTreeView", _treeViewPage, _viewKey, ElaIconType::ListTree);
    expandNavigationNode(_viewKey);

    addPageNode("ElaGraphics", _graphicsPage, 9, ElaIconType::Paintbrush);
    addPageNode("ElaCard", _cardPage, ElaIconType::Cards);
    addPageNode("ElaNavigation", _navigationPage, ElaIconType::LocationArrow);
    addPageNode("ElaPopup", _popupPage, ElaIconType::Envelope);
    addPageNode("ElaIcon", _iconPage, 99, ElaIconType::FontCase);
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
    _aboutPage = new T_About();

    _aboutPage->hide();
    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey) {
        if (_aboutKey == nodeKey)
        {
            _aboutPage->moveToCenter();
            _aboutPage->show();
        }
    });
    addFooterNode("Setting", _settingPage, _settingKey, 0, ElaIconType::GearComplex);
    connect(this, &MainWindow::userInfoCardClicked, this, [=]() {
        this->navigation(_homePage->property("ElaPageKey").toString());
    });
#ifdef Q_OS_WIN
    connect(_homePage, &T_Home::elaScreenNavigation, this, [=]() {
        this->navigation(_elaScreenPage->property("ElaPageKey").toString());
    });
#endif
    connect(_homePage, &T_Home::elaBaseComponentNavigation, this, [=]() {
        this->navigation(_baseComponentsPage->property("ElaPageKey").toString());
    });
    connect(_homePage, &T_Home::elaSceneNavigation, this, [=]() {
        this->navigation(_graphicsPage->property("ElaPageKey").toString());
    });
    connect(_homePage, &T_Home::elaIconNavigation, this, [=]() {
        this->navigation(_iconPage->property("ElaPageKey").toString());
    });
    connect(_homePage, &T_Home::elaCardNavigation, this, [=]() {
        this->navigation(_cardPage->property("ElaPageKey").toString());
    });
    qDebug() << "已注册的事件列表" << ElaEventBus::getInstance()->getRegisteredEventsName();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (getCurrentNavigationIndex() != 2)
    {
        switch (event->button())
        {
        case Qt::BackButton:
        {
            this->setCurrentStackIndex(0);
            break;
        }
        case Qt::ForwardButton:
        {
            this->setCurrentStackIndex(1);
            break;
        }
        default:
        {
            break;
        }
        }
    }
    ElaWindow::mouseReleaseEvent(event);
}
