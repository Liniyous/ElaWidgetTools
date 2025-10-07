#include "ElaAppBarPrivate.h"

#include "ElaToolButton.h"
#ifdef Q_OS_WIN
#include <Windows.h>
#endif
#include "ElaAppBar.h"
#include "ElaIconButton.h"
#include "ElaNavigationBar.h"
#include "ElaText.h"
#include <QDebug>
#include <QGuiApplication>
#include <QLabel>
#include <QMenu>
#include <QPropertyAnimation>
#include <QScreen>
#include <QVBoxLayout>
#include <QWidget>

ElaAppBarPrivate::ElaAppBarPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaAppBarPrivate::~ElaAppBarPrivate()
{
}

void ElaAppBarPrivate::onMinButtonClicked()
{
    Q_Q(ElaAppBar);
    q->window()->showMinimized();
}

void ElaAppBarPrivate::onMaxButtonClicked()
{
    Q_Q(ElaAppBar);
    bool isMaximized = q->window()->isMaximized();
    isMaximized ? q->window()->showNormal() : q->window()->showMaximized();
#ifndef Q_OS_WIM
    _changeMaxButtonAwesome(!isMaximized);
#endif
}

void ElaAppBarPrivate::onCloseButtonClicked()
{
    Q_Q(ElaAppBar);
    if (_pIsDefaultClosed)
    {
        q->window()->close();
    }
    else
    {
        Q_EMIT q_ptr->closeButtonClicked();
    }
}

void ElaAppBarPrivate::onStayTopButtonClicked()
{
#ifdef Q_OS_WIN
    HWND hwnd = (HWND)_currentWinID;
    ::SetWindowPos(hwnd, _pIsStayTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#else
    Q_Q(const ElaAppBar);
    bool isVisible = q->window()->isVisible();
    q->window()->setWindowFlag(Qt::WindowStaysOnTopHint, _pIsStayTop);
    if (isVisible)
    {
        q->window()->show();
    }
#endif
    _stayTopButton->setIsSelected(_pIsStayTop);
    _stayTopButton->update();
}

void ElaAppBarPrivate::_changeMaxButtonAwesome(bool isMaximized)
{
    if (isMaximized)
    {
        _maxButton->setElaIcon(ElaIconType::WindowRestore);
    }
    else
    {
        _maxButton->setElaIcon(ElaIconType::Square);
    }
}

void ElaAppBarPrivate::_showAppBarMenu(QPoint point)
{
    Q_Q(const ElaAppBar);
    if (_pCustomMenu)
    {
        _pCustomMenu->exec(point);
    }
    else
    {
#ifdef Q_OS_WIN
        QScreen* screen = qApp->screenAt(QCursor::pos());
        if (!screen)
        {
            screen = QGuiApplication::primaryScreen();
        }
        if (!screen)
        {
            return;
        }
        const QPoint origin = screen->geometry().topLeft();
        auto nativePos = QPointF(QPointF(point - origin) * screen->devicePixelRatio()).toPoint() + origin;
        HWND hwnd = reinterpret_cast<HWND>(q->window()->winId());
        const HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
        if (q->window()->isMaximized() || q->window()->isFullScreen())
        {
            ::EnableMenuItem(hMenu, SC_MOVE, MFS_DISABLED);
            ::EnableMenuItem(hMenu, SC_RESTORE, MFS_ENABLED);
        }
        else
        {
            ::EnableMenuItem(hMenu, SC_MOVE, MFS_ENABLED);
            ::EnableMenuItem(hMenu, SC_RESTORE, MFS_DISABLED);
        }
        if (!_pIsFixedSize && !q->window()->isMaximized() && !q->window()->isFullScreen())
        {
            ::EnableMenuItem(hMenu, SC_SIZE, MFS_ENABLED);
            ::EnableMenuItem(hMenu, SC_MAXIMIZE, MFS_ENABLED);
        }
        else
        {
            ::EnableMenuItem(hMenu, SC_SIZE, MFS_DISABLED);
            ::EnableMenuItem(hMenu, SC_MAXIMIZE, MFS_DISABLED);
        }
        const int result = ::TrackPopupMenu(hMenu, (TPM_RETURNCMD | (QGuiApplication::isRightToLeft() ? TPM_RIGHTALIGN : TPM_LEFTALIGN)), nativePos.x(),
                                            nativePos.y(), 0, hwnd, nullptr);
        if (result != FALSE)
        {
            ::PostMessageW(hwnd, WM_SYSCOMMAND, result, 0);
        }
#endif
    }
}

void ElaAppBarPrivate::_updateCursor(int edges)
{
    Q_Q(const ElaAppBar);
    switch (edges)
    {
    case 0:
    {
        q->window()->setCursor(Qt::ArrowCursor);
        break;
    }
    case Qt::LeftEdge:
    case Qt::RightEdge:
    {
        q->window()->setCursor(Qt::SizeHorCursor);
        break;
    }
    case Qt::TopEdge:
    case Qt::BottomEdge:
    {
        q->window()->setCursor(Qt::SizeVerCursor);
        break;
    }
    case Qt::LeftEdge | Qt::TopEdge:
    case Qt::RightEdge | Qt::BottomEdge:
    {
        q->window()->setCursor(Qt::SizeFDiagCursor);
        break;
    }
    case Qt::RightEdge | Qt::TopEdge:
    case Qt::LeftEdge | Qt::BottomEdge:
    {
        q->window()->setCursor(Qt::SizeBDiagCursor);
        break;
    }
    default:
    {
        break;
    }
    }
}

bool ElaAppBarPrivate::_containsCursorToItem(QWidget* item)
{
    Q_Q(const ElaAppBar);
    if (!item || !item->isVisible())
    {
        return false;
    }
    auto point = item->window()->mapFromGlobal(QCursor::pos());
    QRectF rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
    if (item == q)
    {
        if (_containsCursorToItem(_routeBackButton) || _containsCursorToItem(_routeForwardButton) || _containsCursorToItem(_navigationButton) || _containsCursorToItem(_pCustomWidget) || _containsCursorToItem(_stayTopButton) || _containsCursorToItem(_themeChangeButton) || _containsCursorToItem(_minButton) || _containsCursorToItem(_maxButton) || _containsCursorToItem(_closeButton))
        {
            return false;
        }
    }
    else if (item == _maxButton)
    {
        rect.adjust(0, 8, 0, 0);
    }
    if (rect.contains(point))
    {
        return true;
    }
    return false;
}

void ElaAppBarPrivate::_onThemeModeChange(ElaThemeType::ThemeMode themeMode)
{
    if (themeMode == ElaThemeType::Light)
    {
        _themeChangeButton->setElaIcon(ElaIconType::MoonStars);
    }
    else
    {
        _themeChangeButton->setElaIcon(ElaIconType::SunBright);
    }
}

int ElaAppBarPrivate::_calculateMinimumWidth()
{
    Q_Q(ElaAppBar);
    int width = 0;
    if (_titleLabel->isVisible())
    {
        width += _titleLabel->width();
        width += 10;
    }
    if (_iconLabel->isVisible())
    {
        width += _iconLabel->width();
        width += 10;
    }
    bool isHasNavigationBar = false;
    if (q->parentWidget()->findChild<ElaNavigationBar*>())
    {
        isHasNavigationBar = true;
        width += 305;
    }
    else
    {
        width += 5;
    }
    if (_pCustomWidget)
    {
        int customWidgetWidth = _pCustomWidget->width();
        if (isHasNavigationBar)
        {
            if (customWidgetWidth > 300)
            {
                width += customWidgetWidth - 300;
            }
        }
        else
        {
            width += customWidgetWidth;
        }
    }
    QList<QAbstractButton*> buttonList = q->findChildren<QAbstractButton*>();
    for (auto button: buttonList)
    {
        if (button->isVisible() && button->objectName() != "NavigationButton")
        {
            width += button->width();
        }
    }
    return width;
}

QVBoxLayout* ElaAppBarPrivate::_createVLayout(QWidget* widget)
{
    if (!widget)
    {
        return nullptr;
    }
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    if (widget == _iconLabel || widget == _titleLabel)
    {
        vLayout->addSpacing(6);
    }
    vLayout->addWidget(widget);
    vLayout->addStretch();
    return vLayout;
}