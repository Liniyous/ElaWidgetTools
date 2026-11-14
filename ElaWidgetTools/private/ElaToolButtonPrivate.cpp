#include "ElaToolButtonPrivate.h"

#include "ElaMenu.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"

#include <QGuiApplication>
#include <QScreen>
#include <QStyleOptionToolButton>
#include <QTimer>

ElaToolButtonPrivate::ElaToolButtonPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaToolButtonPrivate::~ElaToolButtonPrivate()
{
}

void ElaToolButtonPrivate::onMenuDelayTimeout()
{
    Q_Q(ElaToolButton);
    QPointer<ElaToolButton> button(q);
    QPointer<QMenu> popupMenu = _pendingMenu;
    _pendingMenu.clear();
    if (!popupMenu || q->menu() != popupMenu || !q->isEnabled() || !q->isVisible() || popupMenu->isVisible())
    {
        return;
    }
    const bool isInstantPopup = q->popupMode() == QToolButton::InstantPopup;
    if (!isInstantPopup && q->popupMode() != QToolButton::MenuButtonPopup)
    {
        return;
    }
    // 延迟结束后沿用 Qt 的菜单执行链 保留动作转发与关闭后的按钮复位
    Q_EMIT q->pressed();
    if (button && popupMenu && !isInstantPopup && button->menu() == popupMenu)
    {
        button->QToolButton::showMenu();
    }
    if (button)
    {
        button->setDown(false);
        Q_EMIT button->released();
    }
}

bool ElaToolButtonPrivate::_getIsMenuButtonHit(const QPoint& position) const
{
    Q_Q(const ElaToolButton);
    if (!q->isEnabled() || !q->menu() || !q->rect().contains(position))
    {
        return false;
    }
    if (q->popupMode() == QToolButton::InstantPopup)
    {
        return true;
    }
    if (q->popupMode() == QToolButton::MenuButtonPopup)
    {
        QStyleOptionToolButton option;
        q->initStyleOption(&option);
        return q->style()->subControlRect(QStyle::CC_ToolButton, &option, QStyle::SC_ToolButtonMenu, q).contains(position);
    }
    return false;
}

void ElaToolButtonPrivate::_queueMenu()
{
    Q_Q(ElaToolButton);
    _pendingMenu = q->menu();
    _menuDelayTimer->start();
}

void ElaToolButtonPrivate::_cancelMenu()
{
    Q_Q(ElaToolButton);
    _menuDelayTimer->stop();
    _pendingMenu.clear();
    _isMenuPress = false;
    _menuPressKey = 0;
    q->setDown(false);
}

void ElaToolButtonPrivate::_positionMenu()
{
    Q_Q(ElaToolButton);
    QMenu* popupMenu = q->menu();
    if (!popupMenu)
    {
        return;
    }
    const QRect buttonRect(q->mapToGlobal(QPoint()), q->size());
    QScreen* screen = QGuiApplication::screenAt(buttonRect.center());
    if (!screen)
    {
        screen = QGuiApplication::primaryScreen();
    }
    if (!screen)
    {
        return;
    }
    const QRect available = screen->availableGeometry().adjusted(4, 4, -4, -4);
    const QMargins margins = qobject_cast<ElaMenu*>(popupMenu) ? eTheme->getShadowMargins() : QMargins();
    const QSize menuSize = popupMenu->size();
    int x = buttonRect.left() + (buttonRect.width() - menuSize.width()) / 2;
    int y = buttonRect.bottom() + 5 - margins.top();
    if (y + menuSize.height() > available.bottom() + 1)
    {
        y = buttonRect.top() - 4 - menuSize.height() + margins.bottom();
    }
    x = qBound(available.left(), x, qMax(available.left(), available.right() - menuSize.width() + 1));
    y = qBound(available.top(), y, qMax(available.top(), available.bottom() - menuSize.height() + 1));
    // ElaMenu 的显示事件会扣除阴影留白 此处传入内容原点
    popupMenu->move(x + margins.left(), y + margins.top());
}
