#include "mainwindow.h"

#include "ElaActionCommander.h"
#include "ElaApplication.h"
#include "ElaContentDialog.h"
#include "ElaDockWidget.h"
#include "ElaEventBus.h"
#include "ElaLog.h"
#include "ElaMenu.h"
#include "ElaMenuBar.h"
#include "ElaProgressBar.h"
#include "ElaProgressRing.h"
#include "ElaStatusBar.h"
#include "ElaSuggestBox.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolBar.h"
#include "ElaToolButton.h"
#include "ExamplePage/T_ElaScreen.h"
#include "T_About.h"
#include "T_BaseComponents.h"
#include "T_Card.h"
#include "T_ListView.h"
#include "T_Setting.h"
#include "T_TableView.h"
#include "T_TreeView.h"
#include <QDebug>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QTimer>

#include "ElaRibbonBar.h"
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

    // initRibbon();
    //额外布局
    initEdgeLayout();

    //中心窗口
    initContent();

    // 拦截默认关闭事件
    _closeDialog = new ElaContentDialog(this);
    connect(_closeDialog, &ElaContentDialog::rightButtonClicked, this, &MainWindow::close);
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
    setWindowTitle("ElaWidgetTools");
    // setIsStayTop(true);
    // setUserInfoCardVisible(false);
    // setNavigationBarWidth(260);
    ElaText* centralStack = new ElaText("这是一个主窗口堆栈页面", this);
    centralStack->setFocusPolicy(Qt::StrongFocus);
    centralStack->setTextPixelSize(32);
    centralStack->setAlignment(Qt::AlignCenter);
    addCentralWidget(centralStack);

    // 窗口绘制模式
    setWindowPixmap(ElaThemeType::Light, QPixmap(":/Resource/Image/WindowBase/Miku.png"));
    setWindowPixmap(ElaThemeType::Dark, QPixmap(":/Resource/Image/WindowBase/WorldTree.jpg"));
    setWindowMoviePath(ElaThemeType::Light, ":/Resource/Image/WindowBase/Miku.gif");
    setWindowMoviePath(ElaThemeType::Dark, ":/Resource/Image/WindowBase/WorldTree.gif");
    //setWindowPaintMode(ElaWindowType::PaintMode::Pixmap);

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
        ElaActionCommander::getInstance()->undoCommand("ElaWidgetToolsAction");
    });
    ElaToolButton* rightButton = new ElaToolButton(this);
    rightButton->setElaIcon(ElaIconType::AngleRight);
    rightButton->setEnabled(false);
    connect(rightButton, &ElaToolButton::clicked, this, [=]() {
        ElaActionCommander::getInstance()->redoCommand("ElaWidgetToolsAction");
    });
    connect(ElaActionCommander::getInstance(), &ElaActionCommander::commanderStateChanged, this, [=](const QString& domainName, ElaActionCommanderType::CommanderState state) {
        if (domainName != "ElaWidgetToolsAction")
        {
            return;
        }
        switch (state)
        {
        case ElaActionCommanderType::UndoValid:
        {
            leftButton->setEnabled(true);
            break;
        }
        case ElaActionCommanderType::UndoInvalid:
        {
            leftButton->setEnabled(false);
            break;
        }
        case ElaActionCommanderType::RedoValid:
        {
            rightButton->setEnabled(true);
            break;
        }
        case ElaActionCommanderType::RedoInvalid:
        {
            rightButton->setEnabled(false);
            break;
        }
        }
    });
    _windowSuggestBox = new ElaSuggestBox(this);
    _windowSuggestBox->setFixedHeight(32);
    _windowSuggestBox->setPlaceholderText("搜索关键字");
    connect(_windowSuggestBox, &ElaSuggestBox::suggestionClicked, this, [=](const ElaSuggestBox::SuggestData& suggestData) {
        navigation(suggestData.getSuggestData().value("ElaPageKey").toString());
    });

    ElaText* progressBusyRingText = new ElaText("系统运行中", this);
    progressBusyRingText->setIsWrapAnywhere(false);
    progressBusyRingText->setTextPixelSize(15);

    ElaProgressRing* progressBusyRing = new ElaProgressRing(this);
    progressBusyRing->setBusyingWidth(4);
    progressBusyRing->setFixedSize(28, 28);
    progressBusyRing->setIsBusying(true);

    centralCustomWidgetLayout->addWidget(leftButton);
    centralCustomWidgetLayout->addWidget(rightButton);
    centralCustomWidgetLayout->addWidget(_windowSuggestBox);
    centralCustomWidgetLayout->addStretch();
    centralCustomWidgetLayout->addWidget(progressBusyRingText);
    centralCustomWidgetLayout->addWidget(progressBusyRing);
    setCentralCustomWidget(centralCustomWidget);
}

void MainWindow::initRibbon()
{
    setAppBarHeight(30);
    setRibbonHeight(75);
    // QWidget* ribbonTabWidget = new QWidget(this);
    // auto ribbonTabLayout = new QVBoxLayout(ribbonTabWidget);
    // ribbonTabLayout->setContentsMargins(10, 10, 0, 0);
    // _ribbonTabBar = new ElaTabBar(this);
    // _ribbonTabBar->setTabSize(QSize(200, 35));
    // ribbonTabLayout->addWidget(_ribbonTabBar);
    // setCustomWidget(ElaAppBarType::CustomArea::LeftArea, ribbonTabWidget, this, "processHitTest");

    _ribbonBar = new ElaRibbonBar(this);
    // _ribbonBar->setRibbonTabBar(_ribbonTabBar);
    _ribbonBar->move(0, getAppBarHeight());
    auto adminAction = _ribbonBar->addRibbonAction("通用操作", "多用户管理", ElaIconType::User);
    ElaMenu* menu = new ElaMenu(this);
    menu->addAction("用户资料");
    menu->addAction("角色管理");
    menu->addAction("访问权限");
    adminAction->setMenu(menu);

    _ribbonBar->addRibbonAction("通用操作", "文件管理", "新建", ElaIconType::FilePlus);
    _ribbonBar->addRibbonAction("通用操作", "文件管理", "打开", ElaIconType::FolderOpen);
    _ribbonBar->addRibbonAction("通用操作", "文件管理", "关闭", ElaIconType::FolderClosed);
    _ribbonBar->addRibbonAction("通用操作", "文件管理", "保存", ElaIconType::FileArrowDown);
    _ribbonBar->addRibbonAction("通用操作", "文件管理", "另存为", ElaIconType::Files);

    // 命令操作
    auto undoAction = _ribbonBar->addRibbonAction("通用操作", "命令操作", "撤销", ElaIconType::RotateLeft);
    connect(undoAction, &QAction::triggered, this, [=]() {
        ElaActionCommander::getInstance()->undoCommand("ElaWidgetToolsAction");
    });
    auto redoAction = _ribbonBar->addRibbonAction("通用操作", "命令操作", "重做", ElaIconType::RotateRight);
    connect(redoAction, &QAction::triggered, this, [=]() {
        ElaActionCommander::getInstance()->redoCommand("ElaWidgetToolsAction");
    });
    undoAction->setEnabled(false);
    redoAction->setEnabled(false);
    connect(ElaActionCommander::getInstance(), &ElaActionCommander::commanderStateChanged, this, [=](const QString& domainName, ElaActionCommanderType::CommanderState state) {
        if (domainName != "ElaWidgetToolsAction")
        {
            return;
        }
        switch (state)
        {
        case ElaActionCommanderType::UndoValid:
        {
            undoAction->setEnabled(true);
            break;
        }
        case ElaActionCommanderType::UndoInvalid:
        {
            undoAction->setEnabled(false);
            break;
        }
        case ElaActionCommanderType::RedoValid:
        {
            redoAction->setEnabled(true);
            break;
        }
        case ElaActionCommanderType::RedoInvalid:
        {
            redoAction->setEnabled(false);
            break;
        }
        }
        _ribbonBar->update();
    });

    // 形状工具
    auto operationAction = _ribbonBar->addRibbonAction("通用操作", "形状工具", "编辑模式", ElaIconType::PenLine);
    auto pointAction = _ribbonBar->addRibbonAction("通用操作", "形状工具", "点形", ElaIconType::LocationPlus);
    auto lineAction = _ribbonBar->addRibbonAction("通用操作", "形状工具", "线形", ElaIconType::ChartLine);
    auto circleAction = _ribbonBar->addRibbonAction("通用操作", "形状工具", "圆形", ElaIconType::Circle);
    auto polygonAction = _ribbonBar->addRibbonAction("通用操作", "形状工具", "多边形", ElaIconType::DrawPolygon);
    auto rectAngleAction = _ribbonBar->addRibbonAction("通用操作", "形状工具", "矩形", ElaIconType::Rectangle);
    connect(_ribbonBar, &ElaRibbonBar::ribbonActionTriggered, this, [=](const QString& pageName, const QString& groupName, QAction* action) {
        qDebug() << pageName << groupName << action->text();
    });

    _ribbonBar->addRibbonAction("记录与回放", "回放控制", "开始", ElaIconType::CirclePlay);
    _ribbonBar->addRibbonAction("记录与回放", "回放控制", "暂停", ElaIconType::CirclePause);
    _ribbonBar->addRibbonAction("记录与回放", "回放控制", "结束", ElaIconType::CircleStop);
    _ribbonBar->addRibbonAction("记录与回放", "回放控制", "减速", ElaIconType::CircleMinus);
    _ribbonBar->addRibbonAction("记录与回放", "回放控制", "加速", ElaIconType::CirclePlus);

    // 系统与设置
    auto pluginAction = _ribbonBar->addRibbonAction("系统与设置", "插件管理", "插件管理", ElaIconType::PlugCirclePlus);
    _ribbonBar->addRibbonAction("系统与设置", "系统设置", "设置", ElaIconType::GearComplex);
    _ribbonBar->switchRibbonPage("通用操作");
}

void MainWindow::initEdgeLayout()
{
    //菜单栏
    ElaMenuBar* menuBar = new ElaMenuBar(this);
    menuBar->setFixedHeight(30);
    QWidget* customWidget = new QWidget(this);
    customWidget->setFixedWidth(450);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(0, 0, 0, 0);
    customLayout->addWidget(menuBar);
    customLayout->addStretch();
    // this->setMenuBar(menuBar);
    this->setCustomWidget(ElaAppBarType::MiddleArea, customWidget);

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

    menuBar->addMenu("文件(&B)")->addElaIconAction(ElaIconType::ArrowRotateRight, "最近打开");
    menuBar->addMenu("编辑(&C)")->addElaIconAction(ElaIconType::ArrowRotateRight, "编辑选项");
    menuBar->addMenu("视图(&E)")->addElaIconAction(ElaIconType::ArrowRotateRight, "视图设置");
    menuBar->addMenu("工具(&F)")->addElaIconAction(ElaIconType::ArrowRotateRight, "工具选项");
    menuBar->addMenu("帮助(&G)")->addElaIconAction(ElaIconType::ArrowRotateRight, "使用说明");

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
    _elaScreenPage = new T_ElaScreen(this);
    _iconPage = new T_Icon(this);
    _baseComponentsPage = new T_BaseComponents(this);
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
    addExpanderNode("ElaDxgi", _elaDxgiKey, ElaIconType::TvMusic);
    QString dxgiCategoryKey;
    addCategoryNode("Windows-DXGI", dxgiCategoryKey, _elaDxgiKey);
    addPageNode("ElaScreen", _elaScreenPage, _elaDxgiKey, 3, ElaIconType::ObjectGroup);
    QString controlCategoryKey;
    addCategoryNode("Controls", controlCategoryKey);
    // navigation(elaScreenWidget->property("ElaPageKey").toString());
    addPageNode("ElaBaseComponents", _baseComponentsPage, ElaIconType::CabinetFiling);

    addExpanderNode("ElaView", _viewKey, ElaIconType::CameraViewfinder);
    QString viewCategoryKey;
    addCategoryNode("View Content", viewCategoryKey, _viewKey);
    addPageNode("ElaListView", _listViewPage, _viewKey, 9, ElaIconType::List);
    addPageNode("ElaTableView", _tableViewPage, _viewKey, ElaIconType::Table);
    addPageNode("ElaTreeView", _treeViewPage, _viewKey, ElaIconType::ListTree);
    expandNavigationNode(_viewKey);
    addPageNode("ElaCard", _cardPage, ElaIconType::Cards);
    QString customKey;
    addCategoryNode("Custom", customKey);
    addPageNode("ElaNavigation", _navigationPage, ElaIconType::LocationArrow);
    addPageNode("ElaPopup", _popupPage, ElaIconType::Envelope);
    addPageNode("ElaIcon", _iconPage, 99, ElaIconType::FontCase);
    addExpanderNode("工作空间", testKey_1, ElaIconType::Acorn);
    addExpanderNode("项目资源", testKey_2, testKey_1, ElaIconType::Acorn);
    addPageNode("资源概览", new QWidget(this), testKey_2, ElaIconType::Acorn);
    for (int i = 0; i < 10; i++)
    {
        addExpanderNode(QString("资源分组 %1").arg(i + 1), testKey_1, testKey_2, ElaIconType::Acorn);
    }
    addExpanderNode("数据管理", testKey_1, ElaIconType::Acorn);
    addExpanderNode("运行监控", testKey_1, ElaIconType::Acorn);
    addExpanderNode("开发资源", testKey_1, ElaIconType::Acorn);

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
    connect(_homePage, &T_Home::elaIconNavigation, this, [=]() {
        this->navigation(_iconPage->property("ElaPageKey").toString());
    });
    connect(_homePage, &T_Home::elaCardNavigation, this, [=]() {
        this->navigation(_cardPage->property("ElaPageKey").toString());
    });

    _windowSuggestBox->addSuggestion(getNavigationSuggestDataList());
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
