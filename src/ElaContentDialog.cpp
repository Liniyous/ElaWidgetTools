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

#include "ElaText.h"
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
    : QDialog{parent}
{
    QList<QWidget*> widgetList = QApplication::topLevelWidgets();
    QWidget* mainWindow = nullptr;
    for (auto widget : widgetList)
    {
        if (widget->isActiveWindow() && widget->objectName() == "ElaWidnow")
        {
            mainWindow = widget;
        }
    }
    if (mainWindow)
    {
        _shadowWidget = new QWidget(mainWindow);
        _shadowWidget->move(0, 0);
        _shadowWidget->setFixedSize(mainWindow->size());
        _shadowWidget->setObjectName("ElaShadowWidget");
        _shadowWidget->setStyleSheet("#ElaShadowWidget{background-color:rgba(0,0,0,90);}");
        _shadowWidget->setVisible(true);
    }
    resize(400, height());
#if (QT_VERSION == QT_VERSION_CHECK(6, 5, 3) || QT_VERSION == QT_VERSION_CHECK(6, 6, 0))
    setWindowFlags((window()->windowFlags()) | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
    installEventFilter(this);
    setShadow((HWND)winId());
#endif
    QGuiApplication::instance()->installNativeEventFilter(this);
    _leftButton = new ElaPushButton("cancel", this);
    connect(_leftButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT leftButtonClicked();
        onLeftButtonClicked(); });
    _leftButton->setMinimumSize(0, 0);
    _leftButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    _leftButton->setFixedHeight(38);
    _leftButton->setBorderRadius(6);
    _middleButton = new ElaPushButton("minimum", this);
    connect(_middleButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT middleButtonClicked();
        onMiddleButtonClicked(); });
    _middleButton->setMinimumSize(0, 0);
    _middleButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    _middleButton->setFixedHeight(38);
    _middleButton->setBorderRadius(6);
    _rightButton = new ElaPushButton("exit", this);
    connect(_rightButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT rightButtonClicked();
        onRightButtonClicked(); });
    _rightButton->setLightDefaultColor(QColor(0x00, 0x66, 0xB4));
    _rightButton->setLightHoverColor(QColor(0x00, 0x70, 0xC6));
    _rightButton->setLightPressColor(QColor(0x00, 0x7A, 0xD8));
    _rightButton->setLightTextColor(Qt::white);
    _rightButton->setDarkDefaultColor(QColor(0x4C, 0xA0, 0xE0));
    _rightButton->setDarkHoverColor(QColor(0x45, 0x91, 0xCC));
    _rightButton->setDarkPressColor(QColor(0x3F, 0x85, 0xBB));
    _rightButton->setDarkTextColor(Qt::black);
    _rightButton->setMinimumSize(0, 0);
    _rightButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    _rightButton->setFixedHeight(38);
    _rightButton->setBorderRadius(6);

    _centralWidget = new QWidget(this);
    QVBoxLayout* centralVLayout = new QVBoxLayout(_centralWidget);
    centralVLayout->setContentsMargins(9, 15, 9, 20);
    ElaText* title = new ElaText("退出", this);
    title->setTextStyle(ElaTextType::Title);
    ElaText* subTitle = new ElaText("确定要退出程序吗", this);
    subTitle->setTextStyle(ElaTextType::Body);
    centralVLayout->addWidget(title);
    centralVLayout->addWidget(subTitle);
    centralVLayout->addStretch();

    _mainLayout = new QVBoxLayout(this);
    _buttonLayout = new QHBoxLayout();
    _buttonLayout->addWidget(_leftButton);
    _buttonLayout->addWidget(_middleButton);
    _buttonLayout->addWidget(_rightButton);
    _mainLayout->addWidget(_centralWidget);
    _mainLayout->addLayout(_buttonLayout);
}

ElaContentDialog::~ElaContentDialog()
{
    if (_shadowWidget)
    {
        delete _shadowWidget;
    }
}

void ElaContentDialog::onLeftButtonClicked()
{
    this->hide();
    this->deleteLater();
}

void ElaContentDialog::onMiddleButtonClicked()
{
}

void ElaContentDialog::onRightButtonClicked()
{
    this->hide();
    this->deleteLater();
}

void ElaContentDialog::setCentralWidget(QWidget* centralWidget)
{
    _mainLayout->takeAt(0);
    _mainLayout->takeAt(0);
    delete _centralWidget;
    _mainLayout->addWidget(centralWidget);
    _mainLayout->addLayout(_buttonLayout);
}

void ElaContentDialog::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    // 背景绘制
    painter.drawRect(rect());
    // 按钮栏背景绘制
    painter.setBrush(QColor(0xF3, 0xF3, 0xF3));
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

bool ElaContentDialog::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result)
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
            window()->setContentsMargins(8, 8, 8, 8);
        }
        else
        {
            window()->setContentsMargins(0, 0, 0, 0);
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
