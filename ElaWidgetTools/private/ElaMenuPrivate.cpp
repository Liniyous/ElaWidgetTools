#include "ElaMenuPrivate.h"

#include "ElaColorAnimation.h"
#include "ElaMenu.h"

#include <QApplication>
#include <QMenuBar>
#include <QPropertyAnimation>
#include <QToolButton>

ElaMenuPrivate::ElaMenuPrivate(QObject* parent)
    : QObject{parent}
{
    _pAnimationImagePosY = 0.0;
}

ElaMenuPrivate::~ElaMenuPrivate()
{
}

QWidget* ElaMenuPrivate::_getPopupSource() const
{
    Q_Q(const ElaMenu);
    QMenu* parentMenu = qobject_cast<QMenu*>(q->parentWidget());
    if (!parentMenu || !parentMenu->isVisible())
    {
        parentMenu = qobject_cast<QMenu*>(QApplication::activePopupWidget());
    }
    if (parentMenu && parentMenu != q && parentMenu->isVisible() && parentMenu->activeAction() && parentMenu->activeAction()->menu() == q)
    {
        return parentMenu;
    }

    auto* parentMenuBar = qobject_cast<QMenuBar*>(q->parentWidget());
    if (parentMenuBar && parentMenuBar->activeAction() != q->menuAction())
    {
        parentMenuBar = nullptr;
    }
    const QList<QWidget*> actionWidgets = q->menuAction() ? q->menuAction()->associatedWidgets() : QList<QWidget*>();
    if (!parentMenuBar)
    {
        for (QWidget* widget: actionWidgets)
        {
            auto* menuBar = qobject_cast<QMenuBar*>(widget);
            if (menuBar && menuBar->isVisible() && menuBar->activeAction() == q->menuAction())
            {
                return menuBar;
            }
        }
    }
    if (parentMenuBar && parentMenuBar->isVisible())
    {
        return parentMenuBar;
    }

    auto* parentToolButton = qobject_cast<QToolButton*>(q->parentWidget());
    if (parentToolButton && parentToolButton->isVisible() && parentToolButton->menu() == q)
    {
        return parentToolButton;
    }

    const auto isTriggeringToolButton = [q](const QToolButton* toolButton) {
        if (!toolButton || !toolButton->isVisible() || toolButton->menu() != q)
        {
            return false;
        }
        const QRect buttonRect(toolButton->mapToGlobal(QPoint()), toolButton->size());
        const QRect triggerZone = buttonRect.adjusted(-q->width() - 32, -q->height() - 64, q->width() + 32, q->height() + 64);
        return toolButton->isDown() || triggerZone.contains(q->pos());
    };
    // 工具按钮可以关联无父窗口的菜单 用菜单动作的关联控件识别触发源
    for (QWidget* widget: actionWidgets)
    {
        auto* toolButton = qobject_cast<QToolButton*>(widget);
        if (isTriggeringToolButton(toolButton))
        {
            return toolButton;
        }
    }
    if (q->parentWidget())
    {
        const QList<QToolButton*> toolButtons = q->parentWidget()->window()->findChildren<QToolButton*>();
        for (QToolButton* toolButton: toolButtons)
        {
            if (isTriggeringToolButton(toolButton))
            {
                return toolButton;
            }
        }
    }
    return nullptr;
}

void ElaMenuPrivate::_startShowAnimation(bool isSlideDown, int bodyHeight)
{
    Q_Q(ElaMenu);
    if (!ElaColorAnimation::getIsAnimationEnabled(q) || bodyHeight <= 12)
    {
        return;
    }
    _animationPix = q->grab(q->rect());
    _animationFocusWidget = q->focusWidget();
    for (auto* widget: q->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly))
    {
        if (!widget->isWindow() && widget->isVisible())
        {
            _animationWidgets.append(widget);
            widget->hide();
        }
    }

    const int slideDistance = qBound(1, qMin(160, qRound(bodyHeight * 0.55)), bodyHeight - 8);
    const qreal startOffsetY = isSlideDown ? -static_cast<qreal>(slideDistance) : static_cast<qreal>(slideDistance);
    _pAnimationImagePosY = startOffsetY;
    _showAnimation->setStartValue(startOffsetY);
    _showAnimation->start();
}

void ElaMenuPrivate::_finishShowAnimation()
{
    Q_Q(ElaMenu);
    _showAnimation->stop();
    _pAnimationImagePosY = 0.0;
    _animationPix = QPixmap();
    for (const auto& widget: _animationWidgets)
    {
        if (widget)
        {
            widget->show();
        }
    }
    _animationWidgets.clear();
    if (_animationFocusWidget && q->isVisible())
    {
        _animationFocusWidget->setFocus(Qt::PopupFocusReason);
    }
    _animationFocusWidget.clear();
    q->update();
}
