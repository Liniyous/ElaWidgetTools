#include "ElaAppBar.h"

#include <QApplication>
#include <QDebug>

#include "ElaText.h"
#include "ElaToolButton.h"
#include "ElaWinShadowHelper.h"

#ifndef Q_OS_WIN
#include <QDateTime>
#include <QWindow>
#endif
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>

#include "ElaDef.h"
#include "ElaEventBus.h"
#include "ElaIconButton.h"
#include "ElaTheme.h"
#include "private/ElaAppBarPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaAppBar, bool, IsStayTop)
Q_PROPERTY_CREATE_Q_CPP(ElaAppBar, bool, IsDefaultClosed)
Q_PROPERTY_CREATE_Q_CPP(ElaAppBar, bool, IsOnlyAllowMinAndClose)

ElaAppBar::ElaAppBar(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaAppBarPrivate())
{
    Q_D(ElaAppBar);
    d->_buttonFlags = ElaAppBarType::RouteBackButtonHint | ElaAppBarType::RouteForwardButtonHint | ElaAppBarType::StayTopButtonHint | ElaAppBarType::ThemeChangeButtonHint | ElaAppBarType::MinimizeButtonHint | ElaAppBarType::MaximizeButtonHint | ElaAppBarType::CloseButtonHint;
    window()->setAttribute(Qt::WA_Mapped);
    d->_pAppBarHeight = 45;
    setFixedHeight(d->_pAppBarHeight);
    window()->setContentsMargins(0, this->height(), 0, 0);
    d->q_ptr = this;
    d->_pIsStayTop = false;
    d->_pIsFixedSize = false;
    d->_pIsDefaultClosed = true;
    d->_pIsOnlyAllowMinAndClose = false;
    d->_pCustomMenu = nullptr;
    d->_pCustomWidget = nullptr;
    d->_pCustomWidgetMaximumWidth = 550;
    window()->installEventFilter(this);
#ifdef Q_OS_WIN
    if (!eWinHelper->getIsWinVersionGreater10())
    {
        d->_win7Margins = 8;
    }
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    window()->setWindowFlags((window()->windowFlags()) | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
#endif
#else
    window()->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint | Qt::WindowFullscreenButtonHint | Qt::WindowSystemMenuHint);
#endif
    setMouseTracking(true);
    setObjectName("ElaAppBar");
    setStyleSheet("#ElaAppBar{background-color:transparent;}");
    d->_routeBackButton = new ElaToolButton(this);
    d->_routeBackButton->setElaIcon(ElaIconType::ArrowLeft);
    d->_routeBackButton->setFixedSize(35, 30);
    d->_routeBackButton->setEnabled(false);
    // 路由跳转
    connect(d->_routeBackButton, &ElaIconButton::clicked, this, &ElaAppBar::routeBackButtonClicked);

    d->_routeForwardButton = new ElaToolButton(this);
    d->_routeForwardButton->setElaIcon(ElaIconType::ArrowRight);
    d->_routeForwardButton->setFixedSize(35, 30);
    d->_routeForwardButton->setEnabled(false);
    connect(d->_routeForwardButton, &ElaToolButton::clicked, this, &ElaAppBar::routeForwardButtonClicked);

    // 导航栏展开按钮
    d->_navigationButton = new ElaToolButton(this);
    d->_navigationButton->setElaIcon(ElaIconType::Bars);
    d->_navigationButton->setFixedSize(40, 30);
    d->_navigationButton->setObjectName("NavigationButton");
    d->_navigationButton->setVisible(false);
    // 展开导航栏
    connect(d->_navigationButton, &ElaToolButton::clicked, this, &ElaAppBar::navigationButtonClicked);

    // 设置置顶
    d->_stayTopButton = new ElaToolButton(this);
    d->_stayTopButton->setElaIcon(ElaIconType::ArrowUpToArc);
    d->_stayTopButton->setFixedSize(40, 30);
    connect(d->_stayTopButton, &ElaToolButton::clicked, this, [=]() {
        this->setIsStayTop(!this->getIsStayTop());
    });
    connect(this, &ElaAppBar::pIsStayTopChanged, d, &ElaAppBarPrivate::onStayTopButtonClicked);

    //图标
    d->_iconLabel = new QLabel(this);
    d->_iconLabelLayout = d->_createVLayout(d->_iconLabel);
    if (parent->windowIcon().isNull())
    {
        d->_iconLabel->setVisible(false);
    }
    else
    {
        d->_iconLabel->setPixmap(parent->windowIcon().pixmap(18, 18));
        d->_iconLabelLayout->setContentsMargins(10, 0, 0, 0);
    }
    connect(parent, &QWidget::windowIconChanged, this, [=](const QIcon& icon) {
        d->_iconLabel->setPixmap(icon.pixmap(18, 18));
        d->_iconLabel->setVisible(!icon.isNull());
        d->_iconLabelLayout->setContentsMargins(icon.isNull() ? 0 : 10, 0, 0, 0);
    });

    //标题
    d->_titleLabel = new ElaText(this);
    d->_titleLabel->setIsWrapAnywhere(false);
    d->_titleLabel->setTextPixelSize(13);
    d->_titleLabelLayout = d->_createVLayout(d->_titleLabel);
    if (parent->windowTitle().isEmpty())
    {
        d->_titleLabel->setVisible(false);
    }
    else
    {
        d->_titleLabel->setText(parent->windowTitle());
        d->_titleLabelLayout->setContentsMargins(10, 0, 0, 0);
    }
    connect(parent, &QWidget::windowTitleChanged, this, [=](const QString& title) {
        d->_titleLabel->setText(title);
        d->_titleLabel->setVisible(!title.isEmpty());
        d->_titleLabelLayout->setContentsMargins(title.isEmpty() ? 0 : 10, 0, 0, 0);
    });

    // 主题变更
    d->_themeChangeButton = new ElaToolButton(this);
    d->_themeChangeButton->setElaIcon(ElaIconType::MoonStars);
    d->_themeChangeButton->setFixedSize(40, 30);
    connect(d->_themeChangeButton, &ElaToolButton::clicked, this, &ElaAppBar::themeChangeButtonClicked);
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_onThemeModeChange(themeMode);
    });

    d->_minButton = new ElaToolButton(this);
    d->_minButton->setElaIcon(ElaIconType::Dash);
    d->_minButton->setFixedSize(40, 30);
    connect(d->_minButton, &ElaToolButton::clicked, d, &ElaAppBarPrivate::onMinButtonClicked);
    d->_maxButton = new ElaToolButton(this);
    d->_maxButton->setIconSize(QSize(18, 18));
    d->_maxButton->setElaIcon(ElaIconType::Square);
    d->_maxButton->setFixedSize(40, 30);
    connect(d->_maxButton, &ElaToolButton::clicked, d, &ElaAppBarPrivate::onMaxButtonClicked);
    d->_closeButton = new ElaIconButton(ElaIconType::Xmark, 18, 40, 30, this);
    d->_closeButton->setLightHoverColor(QColor(0xE8, 0x11, 0x23));
    d->_closeButton->setDarkHoverColor(QColor(0xE8, 0x11, 0x23));
    d->_closeButton->setLightHoverIconColor(Qt::white);
    d->_closeButton->setDarkHoverIconColor(Qt::white);
    connect(d->_closeButton, &ElaIconButton::clicked, d, &ElaAppBarPrivate::onCloseButtonClicked);

    d->_mainLayout = new QHBoxLayout(this);
    d->_mainLayout->setContentsMargins(0, 0, 0, 0);
    d->_mainLayout->setSpacing(0);
    d->_mainLayout->addLayout(d->_createVLayout(d->_routeBackButton));
    d->_mainLayout->addLayout(d->_createVLayout(d->_routeForwardButton));
    d->_mainLayout->addLayout(d->_createVLayout(d->_navigationButton));
    d->_mainLayout->addLayout(d->_iconLabelLayout);
    d->_mainLayout->addLayout(d->_titleLabelLayout);
    d->_mainLayout->addStretch();
    d->_mainLayout->addStretch();
    d->_mainLayout->addLayout(d->_createVLayout(d->_stayTopButton));
    d->_mainLayout->addLayout(d->_createVLayout(d->_themeChangeButton));
    d->_mainLayout->addLayout(d->_createVLayout(d->_minButton));
    d->_mainLayout->addLayout(d->_createVLayout(d->_maxButton));
    d->_mainLayout->addLayout(d->_createVLayout(d->_closeButton));

#ifdef Q_OS_WIN
    for (int i = 0; i < qApp->screens().count(); i++)
    {
        connect(qApp->screens().at(i), &QScreen::logicalDotsPerInchChanged, this, [=] {
            if (d->_pIsFixedSize)
            {
                HWND hwnd = (HWND)(d->_currentWinID);
                SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_FRAMECHANGED);
            }
        });
    }
    //主屏幕变更处理
    connect(qApp, &QApplication::primaryScreenChanged, this, [=]() {
        HWND hwnd = (HWND)(d->_currentWinID);
        ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
        ::RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
    });
    d->_lastScreen = qApp->screenAt(window()->geometry().center());
#endif

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
}

ElaAppBar::~ElaAppBar()
{
}

void ElaAppBar::setAppBarHeight(int height)
{
    Q_D(ElaAppBar);
    d->_pAppBarHeight = height;
    setFixedHeight(d->_pAppBarHeight);
    window()->setContentsMargins(0, this->height(), 0, 0);
    Q_EMIT pAppBarHeightChanged();
}

int ElaAppBar::getAppBarHeight() const
{
    Q_D(const ElaAppBar);
    return d->_pAppBarHeight;
}

void ElaAppBar::setCustomWidget(ElaAppBarType::CustomArea customArea, QWidget* widget)
{
    Q_D(ElaAppBar);
    if (!widget || widget == this)
    {
        return;
    }
    widget->setMinimumHeight(0);
    widget->setMaximumHeight(height());
    widget->setMaximumWidth(d->_pCustomWidgetMaximumWidth);
    widget->setParent(this);
    if (d->_pCustomWidget)
    {
        d->_mainLayout->removeWidget(d->_pCustomWidget);
    }
    switch (customArea)
    {
    case ElaAppBarType::LeftArea:
    {
        d->_mainLayout->insertWidget(5, widget);
        break;
    }
    case ElaAppBarType::MiddleArea:
    {
        d->_mainLayout->insertWidget(6, widget);
        break;
    }
    case ElaAppBarType::RightArea:
    {
        d->_mainLayout->insertWidget(7, widget);
        break;
    }
    }
    d->_pCustomWidget = widget;
    Q_EMIT customWidgetChanged();
}

QWidget* ElaAppBar::getCustomWidget() const
{
    Q_D(const ElaAppBar);
    return d->_pCustomWidget;
}

void ElaAppBar::setCustomMenu(QMenu* customMenu)
{
    Q_D(ElaAppBar);
    d->_pCustomMenu = customMenu;
    Q_EMIT customMenuChanged();
}

QMenu* ElaAppBar::getCustomMenu() const
{
    Q_D(const ElaAppBar);
    return d->_pCustomMenu;
}

void ElaAppBar::setCustomWidgetMaximumWidth(int width)
{
    Q_D(ElaAppBar);
    d->_pCustomWidgetMaximumWidth = width;
    if (d->_pCustomWidget)
    {
        d->_pCustomWidget->setMaximumWidth(width);
    }
    Q_EMIT pCustomWidgetMaximumWidthChanged();
}

int ElaAppBar::getCustomWidgetMaximumWidth() const
{
    Q_D(const ElaAppBar);
    return d->_pCustomWidgetMaximumWidth;
}

void ElaAppBar::setIsFixedSize(bool isFixedSize)
{
    Q_D(ElaAppBar);
    d->_pIsFixedSize = isFixedSize;
#ifdef Q_OS_WIN
    HWND hwnd = (HWND)d->_currentWinID;
    DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
    if (d->_pIsFixedSize)
    {
        //切换DPI处理
        style &= ~WS_THICKFRAME;
        ::SetWindowLongPtr(hwnd, GWL_STYLE, style);
    }
    else
    {
        ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME);
    }
#else
    bool isVisible = window()->isVisible();
    window()->setWindowFlags((window()->windowFlags()) | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    if (!isFixedSize)
    {
        window()->setWindowFlag(Qt::WindowMaximizeButtonHint);
    }
    if (isVisible)
    {
        window()->show();
    }
#endif
    Q_EMIT pIsFixedSizeChanged();
}

bool ElaAppBar::getIsFixedSize() const
{
    Q_D(const ElaAppBar);
    return d->_pIsFixedSize;
}

void ElaAppBar::setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable)
{
    Q_D(ElaAppBar);
    if (isEnable)
    {
        setWindowButtonFlags(d->_buttonFlags | buttonFlag);
    }
    else
    {
        setWindowButtonFlags(d->_buttonFlags & ~buttonFlag);
    }
}

void ElaAppBar::setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags)
{
    Q_D(ElaAppBar);
    d->_buttonFlags = buttonFlags;
    if (d->_buttonFlags.testFlag(ElaAppBarType::NoneButtonHint))
    {
        d->_routeBackButton->setVisible(false);
        d->_routeForwardButton->setVisible(false);
        d->_navigationButton->setVisible(false);
        d->_stayTopButton->setVisible(false);
        d->_themeChangeButton->setVisible(false);
        d->_minButton->setVisible(false);
        d->_maxButton->setVisible(false);
        d->_closeButton->setVisible(false);
    }
    else
    {
        d->_routeBackButton->setVisible(d->_buttonFlags.testFlag(ElaAppBarType::RouteBackButtonHint));
        d->_routeForwardButton->setVisible(d->_buttonFlags.testFlag(ElaAppBarType::RouteForwardButtonHint));
        d->_navigationButton->setVisible(d->_buttonFlags.testFlag(ElaAppBarType::NavigationButtonHint));
        d->_stayTopButton->setVisible(d->_buttonFlags.testFlag(ElaAppBarType::StayTopButtonHint));
        d->_themeChangeButton->setVisible(d->_buttonFlags.testFlag(ElaAppBarType::ThemeChangeButtonHint));
        d->_minButton->setVisible(d->_buttonFlags.testFlag(ElaAppBarType::MinimizeButtonHint));
        d->_maxButton->setVisible(d->_buttonFlags.testFlag(ElaAppBarType::MaximizeButtonHint));
        d->_closeButton->setVisible(d->_buttonFlags.testFlag(ElaAppBarType::CloseButtonHint));
    }
}

ElaAppBarType::ButtonFlags ElaAppBar::getWindowButtonFlags() const
{
    return d_ptr->_buttonFlags;
}

void ElaAppBar::setRouteBackButtonEnable(bool isEnable)
{
    Q_D(ElaAppBar);
    d->_routeBackButton->setEnabled(isEnable);
}

void ElaAppBar::setRouteForwardButtonEnable(bool isEnable)
{
    Q_D(ElaAppBar);
    d->_routeForwardButton->setEnabled(isEnable);
}

void ElaAppBar::closeWindow()
{
    Q_D(ElaAppBar);
    QPropertyAnimation* closeOpacityAnimation = new QPropertyAnimation(window(), "windowOpacity");
    connect(closeOpacityAnimation, &QPropertyAnimation::finished, this, [=]() {
        window()->close();
    });
    closeOpacityAnimation->setStartValue(1);
    closeOpacityAnimation->setEndValue(0);
    closeOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    closeOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    if (window()->isMaximized() || window()->isFullScreen() || d->_pIsFixedSize)
    {
        return;
    }
    QPropertyAnimation* geometryAnimation = new QPropertyAnimation(window(), "geometry");
    QRect geometry = window()->geometry();
    geometryAnimation->setStartValue(geometry);
    qreal targetWidth = (geometry.width() - d->_lastMinTrackWidth) * 0.7 + d->_lastMinTrackWidth;
    qreal targetHeight = (geometry.height() - window()->minimumHeight()) * 0.7 + window()->minimumHeight();
    geometryAnimation->setEndValue(QRectF(geometry.center().x() - targetWidth / 2, geometry.center().y() - targetHeight / 2, targetWidth, targetHeight));
    geometryAnimation->setEasingCurve(QEasingCurve::InOutSine);
    geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
int ElaAppBar::takeOverNativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
int ElaAppBar::takeOverNativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
    Q_D(ElaAppBar);
    if ((eventType != "windows_generic_MSG") || !message)
    {
        return 0;
    }
    const auto msg = static_cast<const MSG*>(message);
    const HWND hwnd = msg->hwnd;
    if (!hwnd || !msg)
    {
        return 0;
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
            return 1;
        }
        return 0;
    }
    case WM_NCPAINT:
    {
        if (!eWinHelper->getIsCompositionEnabled())
        {
            *result = FALSE;
            return 1;
        }
        else
        {
            return -1;
        }
    }
    case WM_NCACTIVATE:
    {
        if (eWinHelper->getIsCompositionEnabled())
        {
            *result = ::DefWindowProcW(hwnd, WM_NCACTIVATE, wParam, -1);
        }
        else
        {
            *result = TRUE;
        }
        return 1;
    }
    case WM_SIZE:
    {
        if (wParam == SIZE_RESTORED)
        {
            d->_changeMaxButtonAwesome(false);
        }
        else if (wParam == SIZE_MAXIMIZED)
        {
            d->_changeMaxButtonAwesome(true);
        }
        return 0;
    }
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    case WM_SHOWWINDOW:
    {
        if (wParam == FALSE)
        {
            return 0;
        }
        RECT windowRect{};
        ::GetWindowRect(hwnd, &windowRect);
        int windowWidth = windowRect.right - windowRect.left;
        int windowHeight = windowRect.bottom - windowRect.top;
        static UINT swpFlags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE;
        ::SetWindowPos(hwnd, nullptr, 0, 0, windowWidth + 1, windowHeight, swpFlags);
        ::SetWindowPos(hwnd, nullptr, 0, 0, windowWidth, windowHeight, swpFlags);
        return -1;
    }
#endif
    case WM_NCCALCSIZE:
    {
        if (wParam == FALSE)
        {
            return 0;
        }
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
        if (::IsZoomed(hwnd))
        {
            this->move(7, 7);
            window()->setContentsMargins(8, 8 + height(), 8, 8);
        }
        else
        {
            this->move(0, 0);
            window()->setContentsMargins(0, height(), 0, 0);
        }
        *result = 0;
        return 1;
#else
        RECT* clientRect = &((NCCALCSIZE_PARAMS*)(lParam))->rgrc[0];
        if (eWinHelper->getIsWinVersionGreater10())
        {
            const LONG originTop = clientRect->top;
            const LRESULT hitTestResult = ::DefWindowProcW(hwnd, WM_NCCALCSIZE, wParam, lParam);
            if ((hitTestResult != HTERROR) && (hitTestResult != HTNOWHERE))
            {
                *result = static_cast<long>(hitTestResult);
                return 1;
            }
            clientRect->top = originTop;
        }
        if (::IsZoomed(hwnd))
        {
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
            auto geometry = window()->screen()->geometry();
#else
            QScreen* screen = qApp->screenAt(window()->geometry().center());
            QRect geometry;
            if (!screen)
            {
                screen = qApp->screenAt(QCursor::pos());
            }
            geometry = screen->geometry();
#endif
            clientRect->top = geometry.top();
            if (!eWinHelper->getIsWinVersionGreater10())
            {
                quint32 borderThickness = eWinHelper->getResizeBorderThickness(hwnd);
                clientRect->left = geometry.left();
                clientRect->bottom -= borderThickness;
                clientRect->right -= borderThickness;
            }
        }
        *result = WVR_REDRAW;
        return 1;
#endif
    }
    case WM_MOVE:
    {
        QScreen* currentScreen = qApp->screenAt(window()->geometry().center());
        if (currentScreen && currentScreen != d->_lastScreen)
        {
            if (d->_lastScreen)
            {
                ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
                ::RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
            }
            d->_lastScreen = currentScreen;
        }
        break;
    }
    case WM_NCHITTEST:
    {
        if (d->_containsCursorToItem(d->_maxButton))
        {
            if (*result == HTNOWHERE)
            {
                if (!d->_isHoverMaxButton)
                {
                    d->_isHoverMaxButton = true;
                    d->_maxButton->setIsSelected(true);
                    d->_maxButton->update();
                }
                *result = HTZOOM;
            }
            return 1;
        }
        else
        {
            if (d->_isHoverMaxButton)
            {
                d->_isHoverMaxButton = false;
                d->_maxButton->setIsSelected(false);
                d->_maxButton->update();
            }
        }
        POINT nativeLocalPos{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ScreenToClient(hwnd, &nativeLocalPos);
        RECT clientRect{0, 0, 0, 0};
        ::GetClientRect(hwnd, &clientRect);
        auto clientWidth = clientRect.right - clientRect.left;
        auto clientHeight = clientRect.bottom - clientRect.top;
        bool left = nativeLocalPos.x < d->_win7Margins;
        bool right = nativeLocalPos.x > clientWidth - d->_win7Margins;
        bool top = nativeLocalPos.y < d->_margins;
        bool bottom = nativeLocalPos.y > clientHeight - d->_win7Margins;
        *result = HTNOWHERE;
        if (!d->_pIsOnlyAllowMinAndClose && !d->_pIsFixedSize && !window()->isFullScreen() && !window()->isMaximized())
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
            return 1;
        }
        if (d->_containsCursorToItem(this) && !window()->isFullScreen())
        {
            *result = HTCAPTION;
            return 1;
        }
        *result = HTCLIENT;
        return 1;
    }
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* minmaxInfo = reinterpret_cast<MINMAXINFO*>(lParam);
        RECT rect;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
        d->_lastMinTrackWidth = d->_calculateMinimumWidth();
        minmaxInfo->ptMinTrackSize.x = d->_lastMinTrackWidth * qApp->devicePixelRatio();
        minmaxInfo->ptMinTrackSize.y = parentWidget()->minimumHeight() * qApp->devicePixelRatio();
        minmaxInfo->ptMaxPosition.x = rect.left;
        minmaxInfo->ptMaxPosition.y = rect.top;
        return 1;
    }
    case WM_LBUTTONDBLCLK:
    {
        QVariantMap postData;
        postData.insert("WMClickType", ElaAppBarType::WMLBUTTONDBLCLK);
        ElaEventBus::getInstance()->post("WMWindowClicked", postData);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        QVariantMap postData;
        postData.insert("WMClickType", ElaAppBarType::WMLBUTTONDOWN);
        ElaEventBus::getInstance()->post("WMWindowClicked", postData);
        return 0;
    }
    case WM_LBUTTONUP:
    {
        QVariantMap postData;
        postData.insert("WMClickType", ElaAppBarType::WMLBUTTONUP);
        ElaEventBus::getInstance()->post("WMWindowClicked", postData);
        return 0;
    }
    case WM_NCLBUTTONDOWN:
    {
        QVariantMap postData;
        postData.insert("WMClickType", ElaAppBarType::WMNCLBUTTONDOWN);
        ElaEventBus::getInstance()->post("WMWindowClicked", postData);
        if (d->_containsCursorToItem(d->_maxButton) || d->_pIsOnlyAllowMinAndClose)
        {
            return 1;
        }
        break;
    }
    case WM_NCLBUTTONUP:
    {
        QVariantMap postData;
        postData.insert("WMClickType", ElaAppBarType::WMNCLBUTTONDOWN);
        ElaEventBus::getInstance()->post("WMWindowClicked", postData);
        if (d->_containsCursorToItem(d->_maxButton) && !d->_pIsOnlyAllowMinAndClose)
        {
            d->onMaxButtonClicked();
            return 1;
        }
        break;
    }
    case WM_NCLBUTTONDBLCLK:
    {
        if (!d->_pIsOnlyAllowMinAndClose && !d->_pIsFixedSize)
        {
            return 0;
        }
        return 1;
    }
    case WM_NCRBUTTONDOWN:
    {
        if (wParam == HTCAPTION && !d->_pIsOnlyAllowMinAndClose)
        {
            d->_showAppBarMenu(QCursor::pos());
        }
        break;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        if ((GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(VK_SPACE) & 0x8000) && !d->_pIsOnlyAllowMinAndClose)
        {
            auto pos = window()->geometry().topLeft();
            d->_showAppBarMenu(QPoint(pos.x(), pos.y() + this->height()));
        }
        break;
    }
    }
    return -1;
}
#endif

bool ElaAppBar::eventFilter(QObject* obj, QEvent* event)
{
    Q_D(ElaAppBar);
    switch (event->type())
    {
    case QEvent::Resize:
    {
        QSize size = parentWidget()->size();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
        if (::IsZoomed((HWND)d->_currentWinID))
        {
            this->resize(size.width() - 14, this->height());
        }
        else
        {
            this->resize(size.width(), this->height());
        }
#else
        this->resize(size.width(), this->height());
#endif
        break;
    }
#ifdef Q_OS_WIN
    case QEvent::Show:
    {
        if (!d->_pIsFixedSize && !d->_pIsOnlyAllowMinAndClose)
        {
            HWND hwnd = (HWND)d->_currentWinID;
            DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
            style &= ~WS_SYSMENU;
            ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME);
            if (!eWinHelper->getIsWinVersionGreater10())
            {
                SetClassLong(hwnd, GCL_STYLE, GetClassLong(hwnd, GCL_STYLE) | CS_DROPSHADOW);
            }
        }
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
        HWND hwnd = (HWND)d->_currentWinID;
        ElaWinShadowHelper::getInstance()->setWindowShadow(d->_currentWinID);
        DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
        bool hasCaption = (style & WS_CAPTION) == WS_CAPTION;
        if (!hasCaption)
        {
            ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CAPTION);
        }
#endif
        break;
    }
#endif
    case QEvent::Close:
    {
        QCloseEvent* closeEvent = dynamic_cast<QCloseEvent*>(event);
        if (!d->_pIsDefaultClosed && closeEvent->spontaneous())
        {
            event->ignore();
            if (window()->isMinimized())
            {
                window()->showNormal();
            }
            d->onCloseButtonClicked();
            return true;
        }
        else
        {
            break;
        }
    }
#ifndef Q_OS_WIN
    case QEvent::MouseButtonPress:
    {
        if (d->_edges != 0)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                d->_updateCursor(d->_edges);
                window()->windowHandle()->startSystemResize(Qt::Edges(d->_edges));
            }
        }
        else
        {
            if (d->_containsCursorToItem(this))
            {
                qint64 clickTimer = QDateTime::currentMSecsSinceEpoch();
                qint64 offset = clickTimer - d->_clickTimer;
                d->_clickTimer = clickTimer;
                if (offset > 300)
                {
                    window()->windowHandle()->startSystemMove();
                }
            }
        }
        break;
    }
    case QEvent::MouseButtonDblClick:
    {
        if (d->_containsCursorToItem(this))
        {
            if (window()->isMaximized())
            {
                window()->showNormal();
            }
            else
            {
                window()->showMaximized();
            }
        }
        break;
    }
    case QEvent::MouseButtonRelease:
    {
        d->_edges = 0;
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::RightButton)
        {
            d->_showAppBarMenu(QCursor::pos());
        }
        break;
    }
    case QEvent::HoverMove:
    {
        if (window()->isMaximized() || window()->isFullScreen())
        {
            break;
        }
        if (d->_pIsFixedSize)
        {
            break;
        }
        QHoverEvent* mouseEvent = static_cast<QHoverEvent*>(event);
        QPoint p =
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            mouseEvent->pos();
#else
            mouseEvent->position().toPoint();
#endif
        if (p.x() >= d->_margins && p.x() <= (window()->width() - d->_margins) && p.y() >= d->_margins && p.y() <= (window()->height() - d->_margins))
        {
            if (d->_edges != 0)
            {
                d->_edges = 0;
                d->_updateCursor(d->_edges);
            }
            break;
        }
        d->_edges = 0;
        if (p.x() < d->_margins)
        {
            d->_edges |= Qt::LeftEdge;
        }
        if (p.x() > (window()->width() - d->_margins))
        {
            d->_edges |= Qt::RightEdge;
        }
        if (p.y() < d->_margins)
        {
            d->_edges |= Qt::TopEdge;
        }
        if (p.y() > (window()->height() - d->_margins))
        {
            d->_edges |= Qt::BottomEdge;
        }
        d->_updateCursor(d->_edges);
        break;
    }
#endif
    default:
    {
        break;
    }
    }
    return QObject::eventFilter(obj, event);
}

#ifdef Q_OS_WIN
void ElaAppBar::paintEvent(QPaintEvent* event)
{
    if (eWinHelper->getIsWinVersionGreater10() && !eWinHelper->getIsWinVersionGreater11())
    {
        Q_D(ElaAppBar);
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::Antialiasing);
        auto borderWidth = eWinHelper->getSystemMetricsForDpi((HWND)d->_currentWinID, SM_CXBORDER);
        painter.setPen(QPen(window()->isActiveWindow() ? ElaThemeColor(d->_themeMode, Win10BorderActive) : ElaThemeColor(d->_themeMode, Win10BorderInactive), borderWidth));
        painter.drawLine(QPoint(0, 0), QPoint(window()->width(), 0));
        painter.restore();
    }
}
#endif
