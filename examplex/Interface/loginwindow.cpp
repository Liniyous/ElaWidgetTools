#include "loginwindow.h"
#include "ui_loginwindow.h"

#include "ElaApplication.h"
#include "ElaTheme.h"
#include "../mainwindow.h"

Q_SINGLETON_CREATE_CPP(LoginWindow)
LoginWindow::LoginWindow(QWidget *parent)
    : ElaWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    setProperty("ElaBaseClassName", "ElaWindow");
    setObjectName("LoginWindow");
    setStyleSheet("QWidget#LoginWindow{background-color:transparent;}");

    resize(750, 450);
    setWindowTitle("登录");

    setWindowButtonFlags(ElaAppBarType::CloseButtonHint | ElaAppBarType::MaximizeButtonHint | ElaAppBarType::MinimizeButtonHint);

    ui->btnLogin->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));
    ui->btnLogin->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    ui->btnLogin->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    ui->btnLogin->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    ui->btnLogin->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    ui->btnLogin->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    ui->btnLogin->setLightTextColor(Qt::white);
    ui->btnLogin->setDarkTextColor(Qt::black);

    ui->label->setTextStyle(ElaTextType::Body);
    ui->label_4->setTextStyle(ElaTextType::Body);
    ui->label_5->setTextStyle(ElaTextType::Body);
    _pixmap = QPixmap(":/Resource/Image/background.jpg");

    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });

    _isEnableMica = eApp->getIsEnableMica();
    connect(eApp, &ElaApplication::pIsEnableMicaChanged, this, [=]() {
        _isEnableMica = eApp->getIsEnableMica();
        update();
    });
    moveToCenter();

    ui->lineEdit_user->setText(eBase->getConf("login", "userName"));
    ui->lineEdit_pass->setText(eBase->getConf("login", "passWord", "", true));
    ui->checkBox->setChecked(eBase->getConf("login", "autoLogin").toInt());

    connect(ui->btnLogin, &ElaLoadButton::timeout, this, [=](){
        ui->lineEdit_user->setEnabled(true);
        ui->lineEdit_pass->setEnabled(true);
        hide();
        MainWindow::getInstance()->show();
    });

    connect(ui->lineEdit_user, &ElaLineEdit::returnPressed, this, &LoginWindow::on_btnLogin_clicked);
    connect(ui->lineEdit_pass, &ElaLineEdit::returnPressed, this, &LoginWindow::on_btnLogin_clicked);
}

LoginWindow::~LoginWindow()
{
    qDebug() << "~LoginWindow()";
    delete ui;
}

void LoginWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(QRect(0, 0, ui->widget_login->x() - 10, height()), _pixmap);
    painter.restore();

    QWidget::paintEvent(event);
}

void LoginWindow::on_btnLogin_clicked()
{
    QString user = ui->lineEdit_user->text().trimmed();
    QString pass = ui->lineEdit_pass->text().trimmed();

    QString tip;
    if (user.isEmpty()) {
        tip = "请输入用户名";
    } else if (pass.isEmpty()) {
        tip = "请输入密码";
    } else if (user != "admin" || pass != "123456") {
        tip = "用户名或密码错误!";
    }
    if (!tip.isEmpty()) {
        ElaMessageBar::error(ElaMessageBarType::Top, "登陆失败", tip, 3000);
        return;
    }
    ui->lineEdit_user->setEnabled(false);
    ui->lineEdit_pass->setEnabled(false);
    ui->btnLogin->setLoading(true, 2000);

    eBase->setConf("login", "userName", ui->lineEdit_user->text());
    eBase->setConf("login", "passWord", ui->lineEdit_pass->text(), true);
    eBase->setConf("login", "autoLogin", ui->checkBox->isChecked());
}

