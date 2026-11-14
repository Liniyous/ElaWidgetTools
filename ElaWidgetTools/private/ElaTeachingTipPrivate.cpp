#include "ElaTeachingTipPrivate.h"

#include "ElaColorAnimation.h"
#include "ElaPushButton.h"
#include "ElaTeachingTip.h"
#include "ElaText.h"
#include "ElaToolButton.h"

#include <QApplication>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QScopedValueRollback>
#include <QScreen>
#include <QScrollArea>
#include <QTimer>
#include <QVBoxLayout>
#include <QVariantAnimation>

ElaTeachingTipPrivate::ElaTeachingTipPrivate()
    : QObject(nullptr)
{
}

ElaTeachingTipPrivate::~ElaTeachingTipPrivate()
{
}

void ElaTeachingTipPrivate::onAnimationFinished()
{
    Q_Q(ElaTeachingTip);
    const auto focusWidget = _animationFocusWidget;
    _restoreContent();
    if (!_isOpen || !_updateGeometry())
    {
        q->hide();
        return;
    }
    if (focusWidget && focusWidget->isVisible())
    {
        focusWidget->setFocus(Qt::OtherFocusReason);
    }
    Q_EMIT q->opened();
}

void ElaTeachingTipPrivate::onLayoutTimeout()
{
    Q_Q(ElaTeachingTip);
    if (q->isVisible() && _animationPix.isNull() && !_updateGeometry())
    {
        q->hide();
    }
}

void ElaTeachingTipPrivate::onCloseButtonClicked()
{
    Q_Q(ElaTeachingTip);
    QPointer<ElaTeachingTip> guard(q);
    q->setIsOpen(false);
    if (guard)
    {
        Q_EMIT q->closeButtonClicked();
    }
}

bool ElaTeachingTipPrivate::eventFilter(QObject* watched, QEvent* event)
{
    Q_Q(ElaTeachingTip);
    if (!q->isVisible() || _isUpdatingGeometry)
    {
        return QObject::eventFilter(watched, event);
    }
    auto* widget = qobject_cast<QWidget*>(watched);
    const bool isInside = _getIsWidgetWithin(widget, q);
    QWidget* anchor = _targetWidget ? _targetWidget.data() : q->parentWidget();
    if (widget == q && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease || event->type() == QEvent::MouseMove))
    {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        const QPoint position = mouseEvent->position().toPoint();
#else
        const QPoint position = mouseEvent->pos();
#endif
        if (_shadowPressWidget || !_bubblePath.contains(position))
        {
            if (_isLightDismissEnabled && event->type() == QEvent::MouseButtonPress)
            {
                q->setIsOpen(false);
                return true;
            }
            QWidget* owner = anchor ? anchor->window() : nullptr;
            const QPoint globalPosition = q->mapToGlobal(position);
            QWidget* receiver = _shadowPressWidget ? _shadowPressWidget.data() : owner ? owner->childAt(owner->mapFromGlobal(globalPosition))
                                                                                       : nullptr;
            if (receiver && !_getIsWidgetWithin(receiver, q))
            {
                if (event->type() == QEvent::MouseButtonPress)
                {
                    _shadowPressWidget = receiver;
                }
                else if (event->type() == QEvent::MouseButtonRelease)
                {
                    _shadowPressWidget.clear();
                }
                // 阴影只表达层级 不遮挡目标控件的点击
                QMouseEvent forwarded(event->type(), receiver->mapFromGlobal(globalPosition), receiver->window()->mapFromGlobal(globalPosition), globalPosition, mouseEvent->button(), mouseEvent->buttons(), mouseEvent->modifiers());
                QCoreApplication::sendEvent(receiver, &forwarded);
                return true;
            }
        }
    }
    const bool isAnchorAncestor = widget && _getIsWidgetWithin(anchor, widget);
    if (isAnchorAncestor && event->type() == QEvent::Hide)
    {
        q->hide();
    }
    else if (isAnchorAncestor && (event->type() == QEvent::Move || event->type() == QEvent::Resize || event->type() == QEvent::ParentChange || event->type() == QEvent::LayoutRequest))
    {
        _layoutTimer->start();
    }
    else if (isInside && event->type() == QEvent::LayoutRequest && _animationPix.isNull())
    {
        _layoutTimer->start();
    }
    else if (_isOpen && _isLightDismissEnabled && ((!isInside && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::Wheel)) || event->type() == QEvent::ApplicationDeactivate))
    {
        q->setIsOpen(false);
    }
    else if (_isOpen && event->type() == QEvent::KeyPress && (isInside || (anchor && widget && widget->window() == anchor->window())))
    {
        const int key = static_cast<QKeyEvent*>(event)->key();
        QWidget* popup = QApplication::activePopupWidget();
        if (popup && _getIsWidgetWithin(popup, q))
        {
            return QObject::eventFilter(watched, event);
        }
        if (key == Qt::Key_Escape)
        {
            q->setIsOpen(false);
            return true;
        }
        if (key == Qt::Key_F6)
        {
            if (isInside && _previousFocusWidget && _previousFocusWidget->isVisible())
            {
                _previousFocusWidget->window()->activateWindow();
                _previousFocusWidget->setFocus(Qt::OtherFocusReason);
            }
            else
            {
                QPointer<ElaTeachingTip> guard(q);
                if (!_animationPix.isNull())
                {
                    onAnimationFinished();
                }
                if (!guard)
                {
                    return true;
                }
                q->activateWindow();
                for (auto* child: q->findChildren<QWidget*>())
                {
                    if (child->isVisible() && child->isEnabled() && (child->focusPolicy() & Qt::TabFocus))
                    {
                        child->setFocus(Qt::TabFocusReason);
                        break;
                    }
                }
            }
            return true;
        }
    }
    return QObject::eventFilter(watched, event);
}

void ElaTeachingTipPrivate::_updateContent()
{
    Q_Q(ElaTeachingTip);
    if (!_animationPix.isNull())
    {
        QPointer<ElaTeachingTip> guard(q);
        onAnimationFinished();
        if (!guard)
        {
            return;
        }
    }
    _titleText->setVisible(!_titleText->text().isEmpty());
    _subTitleText->setVisible(!_subTitleText->text().isEmpty());
    _iconText->setVisible(_iconText->getElaIcon() != ElaIconType::None);
    _actionPushButton->setVisible(!_actionPushButton->text().isEmpty());
    _closePushButton->setVisible(!_closePushButton->text().isEmpty());
    _closeToolButton->setVisible(_closePushButton->text().isEmpty() && !_isLightDismissEnabled);
    _headerWidget->setVisible(!_titleText->isHidden() || !_subTitleText->isHidden() || !_iconText->isHidden() || !_closeToolButton->isHidden());
    _buttonWidget->setVisible(!_actionPushButton->isHidden() || !_closePushButton->isHidden());
    _contentLayout->invalidate();
    _layoutTimer->start();
}

bool ElaTeachingTipPrivate::_getIsWidgetWithin(const QWidget* widget, const QWidget* ancestor)
{
    // QWidget 的 isAncestorOf 不跨越窗口边界 所有权判断需包含子弹窗
    for (auto* current = widget; current; current = current->parentWidget())
    {
        if (current == ancestor)
        {
            return true;
        }
    }
    return false;
}

QRect ElaTeachingTipPrivate::_getAnchorRect() const
{
    Q_Q(const ElaTeachingTip);
    QWidget* anchor = _targetWidget ? _targetWidget.data() : q->parentWidget();
    if (!anchor)
    {
        auto* screen = QGuiApplication::primaryScreen();
        return screen ? screen->availableGeometry() : QRect();
    }
    if (!anchor->isVisible())
    {
        return {};
    }
    if (!_targetWidget)
    {
        anchor = anchor->window();
    }
    QRect rect(anchor->mapToGlobal(QPoint()), anchor->size());
    for (auto* parent = anchor->parentWidget(); parent; parent = parent->parentWidget())
    {
        if (!parent->isVisible())
        {
            return {};
        }
        rect = rect.intersected(QRect(parent->mapToGlobal(QPoint()), parent->size()));
        if (parent->isWindow())
        {
            break;
        }
    }
    return rect;
}

QSize ElaTeachingTipPrivate::_getBodySize(int width) const
{
    const int height = _contentLayout->hasHeightForWidth() ? _contentLayout->totalHeightForWidth(width) : _contentLayout->totalSizeHint().height();
    return QSize(width, qMax(40, height));
}

Qt::Edge ElaTeachingTipPrivate::_getTailEdge(ElaTeachingTipType::PlacementMode placement)
{
    using namespace ElaTeachingTipType;
    switch (placement)
    {
    case Top:
    case TopLeft:
    case TopRight:
    {
        return Qt::BottomEdge;
    }
    case Bottom:
    case BottomLeft:
    case BottomRight:
    {
        return Qt::TopEdge;
    }
    case Left:
    case LeftTop:
    case LeftBottom:
    {
        return Qt::RightEdge;
    }
    case Right:
    case RightTop:
    case RightBottom:
    {
        return Qt::LeftEdge;
    }
    default:
    {
        return static_cast<Qt::Edge>(0);
    }
    }
}

QRect ElaTeachingTipPrivate::_getCandidateRect(ElaTeachingTipType::PlacementMode placement, const QRect& anchor, const QSize& bodySize) const
{
    using namespace ElaTeachingTipType;
    const bool isTail = _targetWidget && _isTailVisible && placement != Center;
    const Qt::Edge edge = _getTailEdge(placement);
    const int tail = isTail ? 8 : 0;
    int x = anchor.center().x() - bodySize.width() / 2;
    int y = anchor.center().y() - bodySize.height() / 2;
    if (!_targetWidget)
    {
        if (placement == Top || placement == TopLeft || placement == TopRight || placement == LeftTop || placement == RightTop)
        {
            y = anchor.top() + 24;
        }
        else if (placement == Bottom || placement == BottomLeft || placement == BottomRight || placement == LeftBottom || placement == RightBottom)
        {
            y = anchor.bottom() - bodySize.height() - 23;
        }
        if (placement == TopLeft || placement == BottomLeft || placement == Left || placement == LeftTop || placement == LeftBottom)
        {
            x = anchor.left() + 24;
        }
        else if (placement == TopRight || placement == BottomRight || placement == Right || placement == RightTop || placement == RightBottom)
        {
            x = anchor.right() - bodySize.width() - 23;
        }
    }
    else if (edge == Qt::TopEdge || edge == Qt::BottomEdge)
    {
        y = edge == Qt::BottomEdge ? anchor.top() - bodySize.height() - tail - 6 : anchor.bottom() + tail + 7;
        if (placement == TopLeft || placement == BottomLeft)
        {
            x = anchor.center().x() - bodySize.width() + 24;
        }
        else if (placement == TopRight || placement == BottomRight)
        {
            x = anchor.center().x() - 24;
        }
    }
    else if (edge == Qt::LeftEdge || edge == Qt::RightEdge)
    {
        x = edge == Qt::RightEdge ? anchor.left() - bodySize.width() - tail - 6 : anchor.right() + tail + 7;
        if (placement == LeftTop || placement == RightTop)
        {
            y = anchor.center().y() - bodySize.height() + 24;
        }
        else if (placement == LeftBottom || placement == RightBottom)
        {
            y = anchor.center().y() - 24;
        }
    }
    return QRect(x - _shadowMargins.left() - (edge == Qt::LeftEdge ? tail : 0), y - _shadowMargins.top() - (edge == Qt::TopEdge ? tail : 0),
                 bodySize.width() + _shadowMargins.left() + _shadowMargins.right() + ((edge == Qt::LeftEdge || edge == Qt::RightEdge) ? tail : 0),
                 bodySize.height() + _shadowMargins.top() + _shadowMargins.bottom() + ((edge == Qt::TopEdge || edge == Qt::BottomEdge) ? tail : 0));
}

bool ElaTeachingTipPrivate::_updateGeometry()
{
    Q_Q(ElaTeachingTip);
    if (_isUpdatingGeometry)
    {
        return true;
    }
    const QScopedValueRollback<bool> updating(_isUpdatingGeometry, true);
    const QRect anchor = _getAnchorRect();
    if (anchor.isEmpty())
    {
        return false;
    }
    QScreen* screen = QGuiApplication::screenAt(anchor.center());
    if (!screen)
    {
        screen = QGuiApplication::primaryScreen();
    }
    if (!screen)
    {
        return false;
    }
    QRect available = screen->availableGeometry().adjusted(8, 8, -8, -8);
    QWidget* owner = _targetWidget ? _targetWidget->window() : q->parentWidget() ? q->parentWidget()->window()
                                                                                 : nullptr;
    if (owner)
    {
        available = available.intersected(QRect(owner->mapToGlobal(QPoint()), owner->size()).adjusted(8, 8, -8, -8));
    }
    const int horizontalSpace = _shadowMargins.left() + _shadowMargins.right() + 8;
    const int verticalSpace = _shadowMargins.top() + _shadowMargins.bottom() + 8;
    if (available.width() < horizontalSpace + 120 || available.height() < verticalSpace + 40)
    {
        return false;
    }
    const int width = qMin(_tipWidth, available.width() - horizontalSpace);
    QSize bodySize = _getBodySize(width);
    bodySize.setHeight(qMin(bodySize.height(), qMin(520, available.height() - verticalSpace)));
    using namespace ElaTeachingTipType;
    QList<PlacementMode> placements{Top, Bottom, Left, Right, TopLeft, TopRight, BottomLeft, BottomRight, LeftTop, LeftBottom, RightTop, RightBottom, Center};
    const auto preferred = _preferredPlacement == Auto ? (_targetWidget ? Top : BottomRight) : _preferredPlacement;
    placements.removeAll(preferred);
    placements.prepend(preferred);
    QRect popupRect;
    for (const auto placement: placements)
    {
        QRect candidate = _getCandidateRect(placement, anchor, bodySize);
        const Qt::Edge edge = _getTailEdge(placement);
        if (edge == Qt::TopEdge || edge == Qt::BottomEdge || placement == Center || !_targetWidget)
        {
            candidate.moveLeft(qBound(available.left(), candidate.left(), available.right() - candidate.width() + 1));
        }
        if (edge == Qt::LeftEdge || edge == Qt::RightEdge || placement == Center || !_targetWidget)
        {
            candidate.moveTop(qBound(available.top(), candidate.top(), available.bottom() - candidate.height() + 1));
        }
        if (available.contains(candidate))
        {
            popupRect = candidate;
            _effectivePlacement = placement;
            break;
        }
    }
    if (popupRect.isEmpty())
    {
        return false;
    }
    const bool isTail = _targetWidget && _isTailVisible && _effectivePlacement != Center;
    const Qt::Edge edge = _getTailEdge(_effectivePlacement);
    const QPoint bodyPosition(_shadowMargins.left() + (isTail && edge == Qt::LeftEdge ? 8 : 0), _shadowMargins.top() + (isTail && edge == Qt::TopEdge ? 8 : 0));
    _bodyRect = QRectF(bodyPosition, bodySize).adjusted(0.5, 0.5, -0.5, -0.5);
    q->setGeometry(popupRect);
    _contentScrollArea->setGeometry(QRect(bodyPosition, bodySize));
    _contentLayout->activate();
    _updateBubblePath(anchor);
    q->update();
    return true;
}

void ElaTeachingTipPrivate::_updateBubblePath(const QRect& anchor)
{
    Q_Q(ElaTeachingTip);
    _bubblePath = QPainterPath();
    _bubblePath.addRoundedRect(_bodyRect, 8, 8);
    _animationOrigin = _bodyRect.center();
    if (!_targetWidget || !_isTailVisible || _effectivePlacement == ElaTeachingTipType::Center)
    {
        return;
    }
    const QPoint point = q->mapFromGlobal(anchor.center());
    const Qt::Edge edge = _getTailEdge(_effectivePlacement);
    QPainterPath tail;
    if (edge == Qt::TopEdge || edge == Qt::BottomEdge)
    {
        const qreal x = qBound(_bodyRect.left() + 24, static_cast<qreal>(point.x()), _bodyRect.right() - 24);
        const qreal y = edge == Qt::TopEdge ? _bodyRect.top() : _bodyRect.bottom();
        const int direction = edge == Qt::TopEdge ? -1 : 1;
        _animationOrigin = QPointF(x, y + direction * 8);
        tail.moveTo(x - 8, y - direction);
        tail.lineTo(_animationOrigin);
        tail.lineTo(x + 8, y - direction);
    }
    else
    {
        const qreal y = qBound(_bodyRect.top() + 16, static_cast<qreal>(point.y()), _bodyRect.bottom() - 16);
        const qreal x = edge == Qt::LeftEdge ? _bodyRect.left() : _bodyRect.right();
        const int direction = edge == Qt::LeftEdge ? -1 : 1;
        _animationOrigin = QPointF(x + direction * 8, y);
        tail.moveTo(x - direction, y - 8);
        tail.lineTo(_animationOrigin);
        tail.lineTo(x - direction, y + 8);
    }
    tail.closeSubpath();
    _bubblePath = _bubblePath.united(tail);
}

void ElaTeachingTipPrivate::_startAnimation(bool isOpening)
{
    Q_Q(ElaTeachingTip);
    _animation->stop();
    if (!ElaColorAnimation::getIsAnimationEnabled(q))
    {
        onAnimationFinished();
        return;
    }
    const bool isContinuing = !_animationPix.isNull();
    if (_animationPix.isNull())
    {
        // 固定最终窗口大小 缩放完整截图 避免自定义子控件参与逐帧布局
        _contentScrollArea->show();
        _contentLayout->activate();
        // Qt 排队更新滚动条显隐 截图前完成本区域布局 防止捕获旧的滚动条状态
        QCoreApplication::sendPostedEvents(_contentScrollArea, QEvent::LayoutRequest);
        QCoreApplication::sendPostedEvents(_contentScrollArea, QEvent::MetaCall);
        _animationFocusWidget = q->focusWidget();
        _animationPix = q->grab();
        _contentScrollArea->hide();
        _animationScale = isOpening ? QSizeF(0.01, 0.01) : QSizeF(1, 1);
    }
    const QSizeF target = isOpening ? QSizeF(1, 1) : QSizeF(20.0 / _bodyRect.width(), 20.0 / _bodyRect.height());
    QEasingCurve easing(QEasingCurve::BezierSpline);
    easing.addCubicBezierSegment(isOpening ? QPointF(0.1, 0.9) : QPointF(0.7, 0), isOpening ? QPointF(0.2, 1) : QPointF(1, 0.5), QPointF(1, 1));
    _animation->setEasingCurve(easing);
    const qreal distance = qMax(qAbs(target.width() - _animationScale.width()), qAbs(target.height() - _animationScale.height()));
    _animation->setDuration(qMax(1, qRound((isOpening ? 300 : 200) * (isContinuing ? distance : 1))));
    _animation->setStartValue(_animationScale);
    _animation->setEndValue(target);
    _animation->start();
}

void ElaTeachingTipPrivate::_restoreContent()
{
    Q_Q(ElaTeachingTip);
    _animation->stop();
    _animationPix = QPixmap();
    _animationScale = QSizeF(1, 1);
    _contentScrollArea->show();
    _animationFocusWidget.clear();
    q->update();
}
