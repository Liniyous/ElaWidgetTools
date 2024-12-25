#include "mainwindow.h"

Q_SINGLETON_CREATE_CPP(MainWindow)
MainWindow::MainWindow(QWidget *parent) : ElaWindow(parent) {

    show();
    QTimer::singleShot(1, this, [=](){
        initWindow();
        initDialog();
        initContent();

        moveToCenter();
    });
    // QTimer::singleShot(10, this, [=](){
    //     eTheme->setThemeMode(static_cast<ElaThemeType::ThemeMode>(DataBase::getInstance()->getConf("theme").toInt()));
    //     eApp->setIsEnableMica(DataBase::getInstance()->getConf("mica").toInt());
    // });
}

MainWindow::~MainWindow()
{
    LoginWindow::getInstance()->close();
}

void MainWindow::initWindow()
{
    QMap<QString, QString> map = eBase->getConfsByGroup("ui");

    setIsCentralStackedWidgetTransparent(map["trans"].toInt());
    resize(1000, 650);
    ElaLog::getInstance()->initMessageLog(map["log"].toInt());

    //setIsNavigationBarEnable(false);
    setNavigationBarDisplayMode(static_cast<ElaNavigationType::NavigationDisplayMode>(map["navBar"].toInt()));
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint | ElaAppBarType::MaximizeButtonHint |
                         ElaAppBarType::MinimizeButtonHint | ElaAppBarType::ThemeChangeButtonHint |
                         ElaAppBarType::RouteBackButtonHint | ElaAppBarType::WindowIconHint |
                         ElaAppBarType::WindowTitleHint);

    setUserInfoCardPixmap(QPixmap(":/Resource/Image/eye.jpg"));
    setUserInfoCardTitle("Todd");
    setUserInfoCardSubTitle("管理员");
    setWindowTitle("主界面");
    // setIsStayTop(true);
    setUserInfoCardVisible(map["user"].toInt() > 0 ? false : true);

    connect(this, &MainWindow::userInfoCardClicked, this, [=](){
        hide();
        LoginWindow::getInstance()->show();
    });
}

void MainWindow::initDialog()
{
    // closeDialog
    _closeDialog = new ElaContentDialog(this);
    _closeDialog->setLeftButtonText("取消");
    _closeDialog->setMiddleButtonText("最小化");
    _closeDialog->setRightButtonText("退出");
    _closeDialog->setParent(this);
    _closeDialog->setWindowModality(Qt::WindowModal);

    connect(_closeDialog, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    connect(_closeDialog, &ElaContentDialog::middleButtonClicked, this, &MainWindow::showMinimized);
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this, [=]() {
        _closeDialog->setMinimumWidth(400);
        _closeDialog->adjustSize();
        QScreen *screen = QGuiApplication::screenAt(geometry().center());
        if (!screen) {
            screen = QGuiApplication::primaryScreen();
        }
        QRect screenGeometry = screen->availableGeometry();
        QPoint centerPoint = geometry().center();
        QPoint dialogTopLeft = centerPoint - QPoint(_closeDialog->frameGeometry().width() / 2, _closeDialog->frameGeometry().height() / 2);

        dialogTopLeft.setX(qBound(screenGeometry.left(), dialogTopLeft.x(), screenGeometry.right() - _closeDialog->width()));
        dialogTopLeft.setY(qBound(screenGeometry.top(), dialogTopLeft.y(), screenGeometry.bottom() - _closeDialog->height()));

        _closeDialog->move(dialogTopLeft);
        _closeDialog->exec();
    });


    connect(_closeDialog, &ElaContentDialog::rightButtonClicked, this, [](){
        eApp->setIsEnableMica(false);
    });

    // loadingDialog
    _loadingDialog = new ElaLoadingDialog(this);
    connect(_loadingDialog, &ElaLoadingDialog::timeout, this, [=](){
        LoginWindow::getInstance()->hide();
        show();
    });
}

void MainWindow::initContent()
{
    _homePage = new T_Home(this);
    _baseComponentsPage = new T_BaseComponents(this);

    _settingPage = new T_Setting(this);

    ElaScrollPage* _timeScrollPage = new ElaScrollPage(this);
    _timeScrollPage->addCentralWidget(C_TimePage::getInstance());

    addPageNode("主页", _homePage, ElaIconType::House);
    addPageNode("基础组件", _baseComponentsPage, ElaIconType::CabinetFiling);
    addPageNode("时间和语言", _timeScrollPage, ElaIconType::Language);
    addFooterNode("设置", _settingPage, _settingKey, 0, ElaIconType::GearComplex);
}

void MainWindow::showLoading(bool loading, int ms)
{
    if (!loading) {
        _loadingDialog->loadingDone();
        return;
    }
    _loadingDialog->setContentsShow("请耐心等待", "正在准备任务中", ms);
    _loadingDialog->adjustSize();
    QScreen *screen = QGuiApplication::screenAt(geometry().center());
    if (!screen) {
        screen = QGuiApplication::primaryScreen();
    }
    QRect screenGeometry = screen->availableGeometry();
    QPoint centerPoint = geometry().center();

    QPoint dialogTopLeft = centerPoint - QPoint(_loadingDialog->frameGeometry().width() / 2, _loadingDialog->frameGeometry().height() / 2);

    dialogTopLeft.setX(qBound(screenGeometry.left(), dialogTopLeft.x(), screenGeometry.right() - _loadingDialog->width()));
    dialogTopLeft.setY(qBound(screenGeometry.top(), dialogTopLeft.y(), screenGeometry.bottom() - _loadingDialog->height()));

    _loadingDialog->move(dialogTopLeft);
    _loadingDialog->exec();
}
