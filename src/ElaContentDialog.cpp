#include "ElaContentDialog.h"

#include <ElaPushButton.h>
#include <Windows.h>
#include <dwmapi.h>
#include <windowsx.h>

#include <QApplication>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>

#include "ElaApplication.h"
#include "ElaText.h"
#include "private/ElaContentDialogPrivate.h"
#if (QT_VERSION == QT_VERSION_CHECK(6, 5, 3) || QT_VERSION == QT_VERSION_CHECK(6, 6, 0))
[[maybe_unused]] static inline void setShadow(HWND hwnd)
{
    const MARGINS shadow = {1, 0, 0, 0};
    typedef HRESULT(WINAPI * DwmExtendFrameIntoClientAreaPtr)(HWND hWnd, const MARGINS* pMarInset);
    HMODULE module = LoadLibraryW(L"dwmapi.dll");
    if (module)
    {
        DwmExtendFrameIntoClientAreaPtr dwm_extendframe_into_client_area_;
        dwm_extendframe_into_client_area_ = reinterpret_cast<DwmExtendFrameIntoClientAreaPtr>(GetProcAddress(module, "DwmExtendFrameIntoClientArea"));
        if (dwm_extendframe_into_client_area_)
        {
            dwm_extendframe_into_client_area_(hwnd, &shadow);
        }
    }
}
#endif

ElaContentDialog::ElaContentDialog(QWidget* parent)
    : QDialog{parent}, d_ptr(new ElaContentDialogPrivate())
{
    Q_D(ElaContentDialog);
    d->q_ptr = this;
    QList<QWidget*> widgetList = QApplication::topLevelWidgets();
    QWidget* mainWindow = nullptr;
    for (auto widget : widgetList)
    {
        if (widget->property("ElaBaseClassName").toString() == "ElaWindow")
        {
            mainWindow = widget;
            break;
        }
    }
    if (mainWindow)
    {
        d->_shadowWidget = new QWidget(mainWindow);
        d->_shadowWidget->move(0, 0);
        d->_shadowWidget->setFixedSize(mainWindow->size());
        d->_shadowWidget->setObjectName("ElaShadowWidget");
        d->_shadowWidget->setStyleSheet("#ElaShadowWidget{background-color:rgba(0,0,0,90);}");
        d->_shadowWidget->setVisible(true);
    }
    resize(400, height());
#if (QT_VERSION == QT_VERSION_CHECK(6, 5, 3) || QT_VERSION == QT_VERSION_CHECK(6, 6, 0))
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags((window()->windowFlags()) | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
    installEventFilter(this);
    createWinId();
    setShadow((HWND)winId());
#endif
    QGuiApplication::instance()->installNativeEventFilter(this);
    setAttribute(Qt::WA_DeleteOnClose);
    d->_leftButton = new ElaPushButton("cancel", this);
    connect(d->_leftButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT leftButtonClicked();
        onLeftButtonClicked();
        close();
    });
    d->_leftButton->setMinimumSize(0, 0);
    d->_leftButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_leftButton->setFixedHeight(38);
    d->_leftButton->setBorderRadius(6);
    d->_middleButton = new ElaPushButton("minimum", this);
    connect(d->_middleButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT middleButtonClicked();
        onMiddleButtonClicked();
        close();
    });
    d->_middleButton->setMinimumSize(0, 0);
    d->_middleButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_middleButton->setFixedHeight(38);
    d->_middleButton->setBorderRadius(6);
    d->_rightButton = new ElaPushButton("exit", this);
    connect(d->_rightButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT rightButtonClicked();
        onRightButtonClicked();
        close();
    });
    d->_rightButton->setLightDefaultColor(QColor(0x00, 0x66, 0xB4));
    d->_rightButton->setLightHoverColor(QColor(0x00, 0x70, 0xC6));
    d->_rightButton->setLightPressColor(QColor(0x00, 0x7A, 0xD8));
    d->_rightButton->setLightTextColor(Qt::white);
    d->_rightButton->setDarkDefaultColor(QColor(0x4C, 0xA0, 0xE0));
    d->_rightButton->setDarkHoverColor(QColor(0x45, 0x91, 0xCC));
    d->_rightButton->setDarkPressColor(QColor(0x3F, 0x85, 0xBB));
    d->_rightButton->setDarkTextColor(Qt::black);
    d->_rightButton->setMinimumSize(0, 0);
    d->_rightButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_rightButton->setFixedHeight(38);
    d->_rightButton->setBorderRadius(6);

    d->_centralWidget = new QWidget(this);
    QVBoxLayout* centralVLayout = new QVBoxLayout(d->_centralWidget);
    centralVLayout->setContentsMargins(9, 15, 9, 20);
    ElaText* title = new ElaText("退出", this);
    title->setTextStyle(ElaTextType::Title);
    ElaText* subTitle = new ElaText("确定要退出程序吗", this);
    subTitle->setTextStyle(ElaTextType::Body);
    centralVLayout->addWidget(title);
    centralVLayout->addWidget(subTitle);
    centralVLayout->addStretch();

    d->_mainLayout = new QVBoxLayout(this);
    d->_buttonLayout = new QHBoxLayout();
    d->_buttonLayout->addWidget(d->_leftButton);
    d->_buttonLayout->addWidget(d->_middleButton);
    d->_buttonLayout->addWidget(d->_rightButton);
    d->_mainLayout->addWidget(d->_centralWidget);
    d->_mainLayout->addLayout(d->_buttonLayout);

    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaContentDialog::~ElaContentDialog()
{
    Q_D(ElaContentDialog);
    QGuiApplication::instance()->removeNativeEventFilter(this);
#if (QT_VERSION == QT_VERSION_CHECK(6, 5, 3) || QT_VERSION == QT_VERSION_CHECK(6, 6, 0))
    removeEventFilter(this);
#endif
    if (d->_shadowWidget)
    {
        delete d->_shadowWidget;
    }
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
    d->_mainLayout->addLayout(d->_buttonLayout);
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

void ElaContentDialog::paintEvent(QPaintEvent* event)
{
    Q_D(ElaContentDialog);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(d->_themeMode == ElaApplicationType::Light ? Qt::white : QColor(0x2B, 0x2B, 0x2B));
    // 背景绘制
    painter.drawRect(rect());
    // 按钮栏背景绘制
    painter.setBrush(d->_themeMode == ElaApplicationType::Light ? QColor(0xF3, 0xF3, 0xF3) : QColor(0x20, 0x20, 0x20));
    painter.drawRoundedRect(QRectF(0, height() - 60, width(), 60), 8, 8);
    painter.restore();
}

#if (QT_VERSION == QT_VERSION_CHECK(6, 5, 3) || QT_VERSION == QT_VERSION_CHECK(6, 6, 0))
[[maybe_unused]] bool ElaContentDialog::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::WindowActivate)
    {
        HWND hwnd = reinterpret_cast<HWND>(window()->winId());
        DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
        bool hasCaption = (style & WS_CAPTION) == WS_CAPTION;
        if (!hasCaption)
        {
            QTimer::singleShot(15, this, [=] { ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CAPTION); });
        }
    }
    return QObject::eventFilter(obj, event);
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool ElaContentDialog::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result)
#else
bool ElaContentDialog::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
#endif
{
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
    case WM_NCCALCSIZE:
    {
#if (QT_VERSION == QT_VERSION_CHECK(6, 5, 3) || QT_VERSION == QT_VERSION_CHECK(6, 6, 0))
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
            // setContentsMargins(0, 0, 0, 0);
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
        else
        {
            const LRESULT hitTestResult = ::DefWindowProcW(hwnd, WM_NCCALCSIZE, wParam, lParam);
            if ((hitTestResult != HTERROR) && (hitTestResult != HTNOWHERE))
            {
                *result = static_cast<long>(hitTestResult);
                return true;
            }
            // qDebug() << clientRect->left << clientRect->top << clientRect->bottom << clientRect->right;
            clientRect->top = 0;
            clientRect->left = 0;
        }
        *result = WVR_REDRAW;
        return true;
#endif
    }
    }
    return QWidget::nativeEvent(eventType, message, result);
}
