#include "ElaLoadingDialog.h"

#include <QApplication>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>

#include "ElaMaskWidget.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaWinShadowHelper.h"
#include "private/ElaLoadingDialogPrivate.h"

ElaLoadingDialog::ElaLoadingDialog(QWidget* parent)
    : QDialog{parent}, d_ptr(new ElaLoadingDialogPrivate())
{
    Q_D(ElaLoadingDialog);
    d->q_ptr = this;

    d->_maskWidget = new ElaMaskWidget(parent);
    d->_maskWidget->move(0, 0);
    d->_maskWidget->setFixedSize(parent->size());
    d->_maskWidget->setVisible(false);

    resize(280, 100);
    setWindowModality(Qt::ApplicationModal);
#ifdef Q_OS_WIN
    createWinId();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    window()->setWindowFlags((window()->windowFlags()) | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
#endif
#else
    window()->setWindowFlags((window()->windowFlags()) | Qt::FramelessWindowHint);
#endif

    d->_centralWidget = new QWidget(this);
    QVBoxLayout* vlay = new QVBoxLayout();
    vlay->setSpacing(2);
    d->_mainTitle = new ElaText("请耐心等待", d->_centralWidget);
    d->_mainTitle->setTextStyle(ElaTextType::Subtitle);
    d->_subTitle = new ElaText("正在准备下载任务中", d->_centralWidget);
    d->_subTitle->setTextStyle(ElaTextType::Body);
    vlay->addWidget(d->_mainTitle);
    vlay->addWidget(d->_subTitle);

    ElaProgressRing* _progressRing = new ElaProgressRing(d->_centralWidget);
    _progressRing->setFixedSize(60, 60);
    QVBoxLayout* rlay = new QVBoxLayout();
    rlay->addStretch();
    rlay->addWidget(_progressRing);
    rlay->addStretch();

    QHBoxLayout* hlay = new QHBoxLayout(d->_centralWidget);
    hlay->setSpacing(25);
    hlay->addLayout(rlay);
    hlay->addLayout(vlay);

    d->_mainLayout = new QVBoxLayout(this);
    d->_mainLayout->setContentsMargins(25, 25, 25, 25);
    d->_mainLayout->addWidget(d->_centralWidget);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });

    d->_outTimer = new QTimer(this);
    d->_outTimer->setSingleShot(true);
    connect(d->_outTimer, &QTimer::timeout, this, [=](){
        d->_maskWidget->doMaskAnimation(0);
        d->_doCloseAnimation();
        emit timeout();
    });
}

ElaLoadingDialog::~ElaLoadingDialog()
{
    Q_D(ElaLoadingDialog);
    d->_maskWidget->deleteLater();
}

void ElaLoadingDialog::setContentsShow(const QString &mainTitle, const QString &subTitle, int ms)
{
    Q_D(ElaLoadingDialog);
    d->_mainTitle->setText(mainTitle);
    d->_subTitle->setText(subTitle);
    d->_ms = ms;
}

void ElaLoadingDialog::showEvent(QShowEvent* event)
{
    Q_D(ElaLoadingDialog);
    d->_maskWidget->setVisible(true);
    d->_maskWidget->raise();
    d->_maskWidget->setFixedSize(parentWidget()->size());
    d->_maskWidget->doMaskAnimation(90);
    d->_outTimer->start(d->_ms);
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

void ElaLoadingDialog::paintEvent(QPaintEvent* event)
{
    Q_D(ElaLoadingDialog);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaThemeColor(d->_themeMode, DialogBase));
    // 背景绘制
    painter.drawRect(rect());
    painter.restore();
}

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool ElaLoadingDialog::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
bool ElaLoadingDialog::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
    Q_D(ElaLoadingDialog);
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

void ElaLoadingDialog::loadingDone()
{
    Q_D(ElaLoadingDialog);
    d->_outTimer->stop();
    d->_maskWidget->doMaskAnimation(0);
    d->_doCloseAnimation();
}
