#include "ElaContentDialog.h"

#include "ElaColorAnimation.h"
#include "ElaMaskWidget.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaWinShadowHelper.h"
#include "private/ElaContentDialogPrivate.h"

#include <ElaPushButton.h>
#include <QApplication>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPainter>
#include <QPointer>
#include <QScreen>
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
    d->_leftButton = new ElaPushButton(d->_leftButtonText, this);
    connect(d->_leftButton, &ElaPushButton::clicked, this, [=]() {
        const QPointer<ElaContentDialog> guard(this);
        onLeftButtonClicked();
        if (!guard)
        {
            return;
        }
        d->_doCloseAnimation(false);
        if (guard)
        {
            Q_EMIT leftButtonClicked();
        }
    });
    d->_leftButton->setMinimumSize(0, 0);
    d->_leftButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_leftButton->setFixedHeight(38);
    d->_leftButton->setBorderRadius(6);
    d->_middleButton = new ElaPushButton(d->_middleButtonText, this);
    connect(d->_middleButton, &ElaPushButton::clicked, this, [=]() {
        const QPointer<ElaContentDialog> guard(this);
        onMiddleButtonClicked();
        if (guard)
        {
            Q_EMIT middleButtonClicked();
        }
    });
    d->_middleButton->setMinimumSize(0, 0);
    d->_middleButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_middleButton->setFixedHeight(38);
    d->_middleButton->setBorderRadius(6);
    d->_rightButton = new ElaPushButton(d->_rightButtonText, this);
    connect(d->_rightButton, &ElaPushButton::clicked, this, [=]() {
        const QPointer<ElaContentDialog> guard(this);
        onRightButtonClicked();
        if (!guard)
        {
            return;
        }
        d->_doCloseAnimation(true);
        if (guard)
        {
            Q_EMIT rightButtonClicked();
        }
    });
    d->_rightButton->setIsAccent(true);
    d->_rightButton->setMinimumSize(0, 0);
    d->_rightButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_rightButton->setFixedHeight(38);
    d->_rightButton->setBorderRadius(6);

    d->_centralWidget = new QWidget(this);
    QVBoxLayout* centralVLayout = new QVBoxLayout(d->_centralWidget);
    centralVLayout->setContentsMargins(24, 24, 24, 24);
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
    d->_mainLayout->setSpacing(0);
    d->_buttonWidget = new QWidget(this);
    d->_buttonWidget->setFixedHeight(70);
    QHBoxLayout* buttonLayout = new QHBoxLayout(d->_buttonWidget);
    buttonLayout->setContentsMargins(24, 16, 24, 16);
    buttonLayout->setSpacing(8);
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

void ElaContentDialog::setLeftButtonText(const QString& text)
{
    Q_D(ElaContentDialog);
    d->_leftButton->setText(text);
}

void ElaContentDialog::setMiddleButtonText(const QString& text)
{
    Q_D(ElaContentDialog);
    d->_middleButton->setText(text);
}

void ElaContentDialog::setRightButtonText(const QString& text)
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
    d->_moveToCenter();
    QDialog::showEvent(event);
}

void ElaContentDialog::paintEvent(QPaintEvent* event)
{
    Q_D(ElaContentDialog);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(d->_themeMode, PopupBase), "Background"));
    // 背景绘制
    painter.drawRect(rect());
    // 按钮栏背景绘制
    painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(d->_themeMode, WindowBase), "LayoutArea"));
    painter.drawRoundedRect(d->_buttonWidget->geometry(), 8, 8);
    painter.restore();
}

void ElaContentDialog::keyPressEvent(QKeyEvent* event)
{
    Q_D(ElaContentDialog);
    switch (event->key())
    {
    case Qt::Key_Escape:
    {
        d->_doCloseAnimation(false);
        break;
    }
    default:
    {
        break;
    }
    }
    event->accept();
}
