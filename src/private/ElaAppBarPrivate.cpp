#include "ElaAppBarPrivate.h"

#include <Windows.h>

#include <QGuiApplication>
#include <QLabel>
#include <QPropertyAnimation>
#include <QWidget>

#include "ElaAppBar.h"
#include "ElaIconButton.h"
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
    if (q->window()->isMaximized())
    {
        q->window()->showNormal();
    }
    else
    {
        q->window()->showMaximized();
    }
}

void ElaAppBarPrivate::onCloseButtonClicked()
{
    Q_Q(ElaAppBar);
    if (_pIsDefaultClosed)
    {
        q->closeWindow();
    }
    else
    {
        Q_EMIT q_ptr->closeButtonClicked();
    }
}

void ElaAppBarPrivate::onStayTopButtonClicked()
{
    Q_Q(const ElaAppBar);
    HWND hwnd = reinterpret_cast<HWND>(q->window()->winId());
    _stayTopButton->setIsSelected(_pIsStayTop);
    ::SetWindowPos(hwnd, _pIsStayTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void ElaAppBarPrivate::_changeMaxButtonAwesome(bool isMaximized)
{
    if (isMaximized)
    {
        _maxButton->setAwesome(ElaIconType::WindowRestore);
    }
    else
    {
        _maxButton->setAwesome(ElaIconType::Square);
    }
}

void ElaAppBarPrivate::_showSystemMenu(QPoint point)
{
    Q_Q(const ElaAppBar);
#ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(q->window()->winId());
    DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
    ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_SYSMENU);
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
    const int result = ::TrackPopupMenu(hMenu, (TPM_RETURNCMD | (QGuiApplication::isRightToLeft() ? TPM_RIGHTALIGN : TPM_LEFTALIGN)), point.x() * q->window()->devicePixelRatio(), point.y() * q->window()->devicePixelRatio(), 0, hwnd, nullptr);
    if (result != FALSE)
    {
        ::PostMessageW(hwnd, WM_SYSCOMMAND, result, 0);
    }
    ::SetWindowLongPtr(hwnd, GWL_STYLE, style & ~WS_SYSMENU);
#endif
}

bool ElaAppBarPrivate::_containsCursorToItem(QWidget* item)
{
    Q_Q(const ElaAppBar);
    if (!item || !item->isVisible())
    {
        return false;
    }
    auto point = QCursor::pos();
    QRectF rect = QRectF(item->mapToGlobal(QPoint(0, 0)), item->size());
    if (item == q)
    {
        if (_containsCursorToItem(_routeBackButton) || _containsCursorToItem(_navigationButton) || _containsCursorToItem(_stayTopButton) || _containsCursorToItem(_themeChangeButton) || _containsCursorToItem(_minButton) || _containsCursorToItem(_maxButton) || _containsCursorToItem(_closeButton))
        {
            return false;
        }
    }
    if (rect.contains(point))
    {
        return true;
    }
    return false;
}

void ElaAppBarPrivate::_onThemeModeChange(ElaApplicationType::ThemeMode themeMode)
{
    if (themeMode == ElaApplicationType::Light)
    {
        _themeChangeButton->setAwesome(ElaIconType::MoonStars);
        QPalette palette = _titleLabel->palette();
        palette.setColor(QPalette::WindowText, Qt::black);
        _titleLabel->setPalette(palette);
    }
    else
    {
        _themeChangeButton->setAwesome(ElaIconType::SunBright);
        QPalette palette = _titleLabel->palette();
        palette.setColor(QPalette::WindowText, Qt::white);
        _titleLabel->setPalette(palette);
    }
}
