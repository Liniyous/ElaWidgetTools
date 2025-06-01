#include "ElaDockWidget.h"

#include <QEvent>
#include <QLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTimer>

#include "ElaApplication.h"
#include "ElaDockWidgetPrivate.h"
#include "ElaDockWidgetTitleBar.h"
#include "ElaTheme.h"
#include "ElaWinShadowHelper.h"

ElaDockWidget::ElaDockWidget(QWidget* parent, Qt::WindowFlags flags)
    : QDockWidget(parent, flags), d_ptr(new ElaDockWidgetPrivate())
{
    Q_D(ElaDockWidget);
    d->q_ptr = this;
    setObjectName("ElaDockWidget");
    d->_titleBar = new ElaDockWidgetTitleBar(this);
    setTitleBarWidget(d->_titleBar);

    // 主题变更动画
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaDockWidgetPrivate::onThemeModeChanged);

    d->_windowDisplayMode = eApp->getWindowDisplayMode();
    connect(eApp, &ElaApplication::pWindowDisplayModeChanged, this, [=]() {
        d->_windowDisplayMode = eApp->getWindowDisplayMode();
        update();
    });
    connect(this, &ElaDockWidget::topLevelChanged, this, [=](bool topLevel) {
        if (eApp->getWindowDisplayMode() == ElaApplicationType::WindowDisplayMode::ElaMica)
        {
            eApp->syncWindowDisplayMode(this, topLevel);
        }
    });
}

ElaDockWidget::ElaDockWidget(const QString& title, QWidget* parent, Qt::WindowFlags flags)
    : ElaDockWidget(parent, flags)
{
    this->setWindowTitle(title);
}

ElaDockWidget::~ElaDockWidget()
{
}

void ElaDockWidget::paintEvent(QPaintEvent* event)
{
    Q_D(ElaDockWidget);
    if (isFloating())
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::Antialiasing);
#ifdef Q_OS_WIN
        // 背景
        if (d->_windowDisplayMode != ElaApplicationType::WindowDisplayMode::ElaMica)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(ElaThemeColor(d->_themeMode, DialogBase));
            painter.drawRect(rect());
        }
#else
        // 背景
        painter.setPen(Qt::NoPen);
        painter.setBrush(d->_windowDisplayMode == ElaApplicationType::WindowDisplayMode::ElaMica ? Qt::transparent : ElaThemeColor(d->_themeMode, DialogBase));
        QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * d->_shadowBorderWidth, height() - 2 * d->_shadowBorderWidth);
        painter.drawRect(rect());
#endif
        painter.restore();
    }
    QDockWidget::paintEvent(event);
}

#ifdef Q_OS_WIN
bool ElaDockWidget::event(QEvent* event)
{
    Q_D(ElaDockWidget);
    switch (event->type())
    {
    case QEvent::Show:
    case QEvent::MouseButtonRelease:
    {
        HWND hwnd = (HWND)d->_currentWinID;
        DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
        ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME);
        ElaWinShadowHelper::getInstance()->setWindowShadow(d->_currentWinID);
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
        bool hasCaption = (style & WS_CAPTION) == WS_CAPTION;
        if (!hasCaption)
        {
            ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CAPTION);
        }
#endif
        break;
    }
    default:
    {
        break;
    }
    }
    return QDockWidget::event(event);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool ElaDockWidget::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
bool ElaDockWidget::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
    Q_D(ElaDockWidget);
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
    case WM_NCHITTEST:
    {
        POINT nativeLocalPos{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ScreenToClient(hwnd, &nativeLocalPos);
        RECT clientRect{0, 0, 0, 0};
        ::GetClientRect(hwnd, &clientRect);
        auto clientWidth = clientRect.right - clientRect.left;
        auto clientHeight = clientRect.bottom - clientRect.top;
        bool left = nativeLocalPos.x < d->_margins;
        bool right = nativeLocalPos.x > clientWidth - d->_margins;
        bool top = nativeLocalPos.y < d->_margins;
        bool bottom = nativeLocalPos.y > clientHeight - d->_margins;
        *result = 0;
        if (!window()->isFullScreen() && !window()->isMaximized())
        {
            if (left && top)
            {
                *result = HTTOPLEFT;
            }
            else if (left && bottom)
            {
                *result = HTBOTTOMLEFT;
            }
            else if (right && top)
            {
                *result = HTTOPRIGHT;
            }
            else if (right && bottom)
            {
                *result = HTBOTTOMRIGHT;
            }
            else if (left)
            {
                *result = HTLEFT;
            }
            else if (right)
            {
                *result = HTRIGHT;
            }
            else if (top)
            {
                *result = HTTOP;
            }
            else if (bottom)
            {
                *result = HTBOTTOM;
            }
        }
        if (0 != *result)
        {
            return true;
        }
        *result = HTCLIENT;
        return true;
    }
    case WM_NCCALCSIZE:
    {
        if (wParam == FALSE)
        {
            return false;
        }
        *result = 0;
        return true;
    }
    }
    return QDockWidget::nativeEvent(eventType, message, result);
}
#endif
