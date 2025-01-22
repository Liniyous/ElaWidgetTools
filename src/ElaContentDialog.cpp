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
#ifdef Q_OS_WIN
    createWinId();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    window()->setWindowFlags((window()->windowFlags()) | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
#endif
#else
    window()->setWindowFlags((window()->windowFlags()) | Qt::FramelessWindowHint);
#endif
    d->_leftButton = new ElaPushButton("cancel", this);
    connect(d->_leftButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT leftButtonClicked();
        onLeftButtonClicked();
        d->_maskWidget->doMaskAnimation(0);
        d->_doCloseAnimation();
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
        d->_doCloseAnimation();
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

void ElaContentDialog::setRightButtonText(QString text) {
    Q_D(ElaContentDialog);
    d->_rightButton->setText(text);
}

void ElaContentDialog::setMiddleButtonVisible(bool visible){
    Q_D(ElaContentDialog);
    d->_middleButton->setVisible(visible);
}

void ElaContentDialog::close()
{
    Q_D(ElaContentDialog);
    d->_doCloseAnimation();
}

void ElaContentDialog::showEvent(QShowEvent* event)
{
    Q_D(ElaContentDialog);
    d->_maskWidget->setVisible(true);
    d->_maskWidget->raise();
    d->_maskWidget->setFixedSize(parentWidget()->size());
    d->_maskWidget->doMaskAnimation(90);
#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    HWND hwnd = (HWND)d->_currentWinID;
    setShadow(hwnd);
    DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
    bool hasCaption = (style & WS_CAPTION) == WS_CAPTION;
    if (!hasCaption)
    {
        ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CAPTION);
    }
#endif
#endif
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

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool ElaContentDialog::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
bool ElaContentDialog::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
    Q_D(ElaContentDialog);
    if ((eventType != "windows_generic_MSG") || !message)
    {
        return false;
    }
    const auto msg = static_cast<const MSG*>(message);
    const HWND hwnd = msg->hwnd;
    if (!hwnd || !msg)
    {
        return false;
    }
    d->_currentWinID = (qint64)hwnd;
    const UINT uMsg = msg->message;
    const WPARAM wParam = msg->wParam;
    const LPARAM lParam = msg->lParam;
    switch (uMsg)
    {
    case WM_WINDOWPOSCHANGING:
    {
        WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(lParam);
        if (wp != nullptr && (wp->flags & SWP_NOSIZE) == 0)
        {
            wp->flags |= SWP_NOCOPYBITS;
            *result = ::DefWindowProcW(hwnd, uMsg, wParam, lParam);
            return true;
        }
        return false;
    }
    case WM_NCACTIVATE:
    {
        *result = TRUE;
        return true;
    }
    case WM_NCCALCSIZE:
    {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
        if (wParam == FALSE)
        {
            return false;
        }
        if (::IsZoomed(hwnd))
        {
            setContentsMargins(8, 8, 8, 8);
        }
        else
        {
            setContentsMargins(0, 0, 0, 0);
        }
        *result = 0;
        return true;
#else
        if (wParam == FALSE)
        {
            return false;
        }
        RECT* clientRect = &((NCCALCSIZE_PARAMS*)(lParam))->rgrc[0];
        if (!::IsZoomed(hwnd))
        {
            clientRect->top -= 1;
            clientRect->bottom -= 1;
        }
        *result = WVR_REDRAW;
        return true;
#endif
    }
    }
    return QDialog::nativeEvent(eventType, message, result);
}
#endif
