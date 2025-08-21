#include "ElaContentDialog.h"

#include <ElaPushButton.h>

#include "ElaMaskWidget.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaWinShadowHelper.h"
#include "private/ElaContentDialogPrivate.h"
#include <QApplication>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPainter>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>
Q_TAKEOVER_NATIVEEVENT_CPP(ElaContentDialog, d_func()->_appBar);
ElaContentDialog::ElaContentDialog(QWidget* parent)
    : QDialog{parent}, d_ptr(new ElaContentDialogPrivate())
{
    Q_D(ElaContentDialog);
    d->q_ptr = this;

    d->_maskWidget = new ElaMaskWidget(parent);
    d->_maskWidget->move(0, 0);
    d->_maskWidget->setFixedSize(parent->size());
    d->_maskWidget->setVisible(false);

    resize(400, height());
    setWindowModality(Qt::ApplicationModal);

    d->_appBar = new ElaAppBar(this);
    d->_appBar->setWindowButtonFlags(ElaAppBarType::NoneButtonHint);
    d->_appBar->setIsFixedSize(true);
    d->_appBar->setAppBarHeight(0);
#ifdef Q_OS_WIN
    // 防止意外拉伸
    createWinId();
#endif
    d->_leftButton = new ElaPushButton("cancel", this);
    connect(d->_leftButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT leftButtonClicked();
        onLeftButtonClicked();
        d->_doCloseAnimation(false);
    });
    d->_leftButton->setMinimumSize(0, 0);
    d->_leftButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_leftButton->setFixedHeight(38);
    d->_leftButton->setBorderRadius(6);
    d->_middleButton = new ElaPushButton("minimum", this);
    connect(d->_middleButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT middleButtonClicked();
        onMiddleButtonClicked();
    });
    d->_middleButton->setMinimumSize(0, 0);
    d->_middleButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_middleButton->setFixedHeight(38);
    d->_middleButton->setBorderRadius(6);
    d->_rightButton = new ElaPushButton("exit", this);
    connect(d->_rightButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT rightButtonClicked();
        onRightButtonClicked();
        d->_doCloseAnimation(true);
    });
    d->_rightButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));
    d->_rightButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    d->_rightButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    d->_rightButton->setLightTextColor(Qt::white);
    d->_rightButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    d->_rightButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    d->_rightButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    d->_rightButton->setDarkTextColor(Qt::black);
    d->_rightButton->setMinimumSize(0, 0);
    d->_rightButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_rightButton->setFixedHeight(38);
    d->_rightButton->setBorderRadius(6);

    d->_centralWidget = new QWidget(this);
    QVBoxLayout* centralVLayout = new QVBoxLayout(d->_centralWidget);
    centralVLayout->setContentsMargins(15, 25, 15, 10);
    ElaText* title = new ElaText("退出", this);
    title->setTextStyle(ElaTextType::Title);
    ElaText* subTitle = new ElaText("确定要退出程序吗", this);
    subTitle->setTextStyle(ElaTextType::Body);
    centralVLayout->addWidget(title);
    centralVLayout->addSpacing(2);
    centralVLayout->addWidget(subTitle);
    centralVLayout->addStretch();

    d->_mainLayout = new QVBoxLayout(this);
    d->_mainLayout->setContentsMargins(0, 0, 0, 0);
    d->_buttonWidget = new QWidget(this);
    d->_buttonWidget->setFixedHeight(60);
    QHBoxLayout* buttonLayout = new QHBoxLayout(d->_buttonWidget);
    buttonLayout->addWidget(d->_leftButton);
    buttonLayout->addWidget(d->_middleButton);
    buttonLayout->addWidget(d->_rightButton);
    d->_mainLayout->addWidget(d->_centralWidget);
    d->_mainLayout->addWidget(d->_buttonWidget);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaContentDialog::~ElaContentDialog()
{
    Q_D(ElaContentDialog);
    d->_maskWidget->deleteLater();
}

void ElaContentDialog::onLeftButtonClicked()
{
}

void ElaContentDialog::onMiddleButtonClicked()
{
}

void ElaContentDialog::onRightButtonClicked()
{
}

void ElaContentDialog::setCentralWidget(QWidget* centralWidget)
{
    Q_D(ElaContentDialog);
    d->_mainLayout->takeAt(0);
    d->_mainLayout->takeAt(0);
    delete d->_centralWidget;
    d->_centralWidget = centralWidget;
    d->_mainLayout->addWidget(centralWidget);
    d->_mainLayout->addWidget(d->_buttonWidget);
}

void ElaContentDialog::setLeftButtonText(QString text)
{
    Q_D(ElaContentDialog);
    d->_leftButton->setText(text);
}

void ElaContentDialog::setMiddleButtonText(QString text)
{
    Q_D(ElaContentDialog);
    d->_middleButton->setText(text);
}

void ElaContentDialog::setRightButtonText(QString text)
{
    Q_D(ElaContentDialog);
    d->_rightButton->setText(text);
}

void ElaContentDialog::close()
{
    Q_D(ElaContentDialog);
    d->_doCloseAnimation(false);
}

void ElaContentDialog::showEvent(QShowEvent* event)
{
    Q_D(ElaContentDialog);
    d->_maskWidget->setVisible(true);
    d->_maskWidget->raise();
    d->_maskWidget->setFixedSize(parentWidget()->size());
    d->_maskWidget->doMaskAnimation(90);
    QDialog::showEvent(event);
}

void ElaContentDialog::paintEvent(QPaintEvent* event)
{
    Q_D(ElaContentDialog);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaThemeColor(d->_themeMode, DialogBase));
    // 背景绘制
    painter.drawRect(rect());
    // 按钮栏背景绘制
    painter.setBrush(ElaThemeColor(d->_themeMode, DialogLayoutArea));
    painter.drawRoundedRect(QRectF(0, height() - 60, width(), 60), 8, 8);
    painter.restore();
}

void ElaContentDialog::keyPressEvent(QKeyEvent* event)
{
    event->accept();
}
