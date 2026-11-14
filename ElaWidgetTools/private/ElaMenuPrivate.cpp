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
    if (!parentMenuBar && q->menuAction())
    {
        for (QWidget* widget: q->menuAction()->associatedWidgets())
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
    if (q->parentWidget())
    {
        const QList<QToolButton*> toolButtons = q->parentWidget()->window()->findChildren<QToolButton*>();
        for (QToolButton* toolButton: toolButtons)
        {
            if (toolButton->isVisible() && toolButton->menu() == q)
            {
                const QRect buttonRect(toolButton->mapToGlobal(QPoint(0, 0)), toolButton->size());
                const QRect triggerZone = buttonRect.adjusted(-q->width() - 32, -q->height() - 64, q->width() + 32, q->height() + 64);
                if (toolButton->isDown() || triggerZone.contains(q->pos()))
                {
                    return toolButton;
                }
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
