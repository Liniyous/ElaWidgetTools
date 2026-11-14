#include "ElaComboBoxPopup.h"

#include "ElaColorAnimation.h"
#include "ElaComboBoxStyle.h"
#include "ElaTheme.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QComboBox>
#include <QFrame>
#include <QKeyEvent>
#include <QLayout>
#include <QScreen>
#include <QScrollBar>
#include <QVariantAnimation>
#include <QWindow>

ElaComboBoxPopup::ElaComboBoxPopup(QComboBox* comboBox)
    : QObject(comboBox)
{
    _comboBox = comboBox;
    _revealAnimation = new QVariantAnimation(this);
    connect(_revealAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
        _applyAnimationFrame(value.toReal());
    });
    connect(_revealAnimation, &QVariantAnimation::finished, this, [this]() {
        if (_isClosing)
        {
            Q_EMIT closeFinished();
        }
        else
        {
            _resetAnimation();
            _comboBox->update();
        }
    });
    connect(_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) {
        if (_isClosing && !_isDropDown && _container && _container->isVisible())
        {
            // 原生容器先调用 hidePopup 再提交新选中索引 此处同步更新收回目标偏移
            _comboTargetRect = _getComboTargetRect();
            _endBodyRect = _comboTargetRect;
            _endOffsetY = _getSelectedOffsetY(_comboTargetRect);
            _applyAnimationFrame(_revealAnimation->currentValue().toReal());
        }
    });
    prepare();
}

void ElaComboBoxPopup::prepare()
{
    _resetAnimation();
    QWidget* container = _comboBox->view()->window();
    if (container == _comboBox->window())
    {
        return;
    }
    if (_container != container)
    {
        if (_container)
        {
            _container->removeEventFilter(this);
        }
        _container = container;
        container->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        container->setAttribute(Qt::WA_TranslucentBackground);
        container->setObjectName("ElaComboBoxContainer");
        container->setStyle(_comboBox->style());
        container->setGraphicsEffect(nullptr);
        container->installEventFilter(this);
    }
    QLayout* layout = container->layout();
    if (layout)
    {
        layout->setEnabled(true);
        // 原生滚动箭头由 Ela 滚动条接管 保留视图的布局所有权
        while (QLayoutItem* item = layout->takeAt(0))
        {
            if (item->widget() && item->widget() != _comboBox->view())
            {
                item->widget()->hide();
            }
            delete item;
        }
        layout->addWidget(_comboBox->view());
        layout->setContentsMargins(eTheme->getShadowMargins());
        layout->setSpacing(0);
    }
    container->setMinimumSize(0, 0);
    container->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    _comboBox->view()->installEventFilter(this);
}

void ElaComboBoxPopup::open(int anchorRow, bool isDropDown)
{
    if (!_container || !_container->isVisible() || _comboBox->count() == 0)
    {
        return;
    }
    _revealAnimation->stop();
    _isClosing = false;
    _isDropDown = isDropDown || _comboBox->isEditable();
    _isOpenUp = false;
    QAbstractItemView* view = _comboBox->view();
    const QRect faceRect(_comboBox->mapToGlobal(QPoint(0, 0)), _comboBox->size());
    QScreen* screen = QGuiApplication::screenAt(faceRect.center());
    if (!screen)
    {
        screen = QGuiApplication::primaryScreen();
    }
    const QRect availableRect = screen->availableGeometry().adjusted(4, 4, -4, -4);
    anchorRow = qBound(0, anchorRow, _comboBox->count() - 1);
    const int visibleRows = qMin(_comboBox->count(), qMax(1, _comboBox->maxVisibleItems()));
    const int firstRow = qBound(0, anchorRow - visibleRows / 2, _comboBox->count() - visibleRows);
    const QMargins margins = eTheme->getShadowMargins();
    int popupHeight = margins.top() + margins.bottom();
    for (int row = firstRow; row < firstRow + visibleRows; ++row)
    {
        const QModelIndex index = _comboBox->model()->index(row, _comboBox->modelColumn(), _comboBox->rootModelIndex());
        popupHeight += qMax(1, view->sizeHintForIndex(index).height());
    }
    popupHeight = qMin(popupHeight, availableRect.height());
    const int popupWidth = qMin(_comboBox->width() + margins.left() + margins.right(), availableRect.width());
    int popupX = _comboBox->layoutDirection() == Qt::RightToLeft ? faceRect.right() - popupWidth + margins.right() + 1 : faceRect.left() - margins.left();
    popupX = qBound(availableRect.left(), popupX, availableRect.right() - popupWidth + 1);
    _container->resize(popupWidth, popupHeight);
    _fullBodyRect = _container->rect().marginsRemoved(margins);
    if (_container->layout())
    {
        _container->layout()->setEnabled(true);
        _container->layout()->activate();
    }
    view->clearMask();
    view->setGeometry(_fullBodyRect);
    view->doItemsLayout();
    const QModelIndex firstIndex = _comboBox->model()->index(firstRow, _comboBox->modelColumn(), _comboBox->rootModelIndex());
    const QModelIndex anchorIndex = _comboBox->model()->index(anchorRow, _comboBox->modelColumn(), _comboBox->rootModelIndex());
    view->scrollTo(firstIndex, QAbstractItemView::PositionAtTop);
    int popupY = 0;
    if (_isDropDown)
    {
        // 多选与可编辑组合框保留主体输入或标签区域 在上下方独立展开
        const int downBodyTop = faceRect.bottom() + 5;
        const int upBodyTop = faceRect.top() - 4 - _fullBodyRect.height();
        if (downBodyTop + _fullBodyRect.height() > availableRect.bottom() + 1 && (faceRect.top() - availableRect.top()) > (availableRect.bottom() - faceRect.bottom()))
        {
            _isOpenUp = true;
            popupY = upBodyTop - margins.top();
        }
        else
        {
            _isOpenUp = false;
            popupY = downBodyTop - margins.top();
        }
    }
    else
    {
        const int itemCenterY = qRound(_fullBodyRect.top() + view->viewport()->y() + QRectF(view->visualRect(anchorIndex)).center().y());
        popupY = faceRect.center().y() - itemCenterY;
    }
    popupY = qBound(availableRect.top(), popupY, availableRect.bottom() - popupHeight + 1);
    _container->move(popupX, popupY);
    _comboTargetRect = _getComboTargetRect();
    if (!ElaColorAnimation::getIsAnimationEnabled(_comboBox) || !QApplication::isEffectEnabled(Qt::UI_AnimateCombo))
    {
        _resetAnimation();
        _comboBox->update();
        return;
    }
    _isAnimating = true;
    QEasingCurve openEasing(QEasingCurve::BezierSpline);
    openEasing.addCubicBezierSegment(QPointF(0, 0), QPointF(0, 1), QPointF(1, 1));
    _revealAnimation->setEasingCurve(openEasing);
    if (_isDropDown)
    {
        const int collapsedHeight = qBound(16, qRound(_fullBodyRect.height() * 0.5), _fullBodyRect.height());
        _startBodyRect = _isOpenUp
            ? QRect(_fullBodyRect.x(), _fullBodyRect.bottom() - collapsedHeight + 1, _fullBodyRect.width(), collapsedHeight)
            : QRect(_fullBodyRect.x(), _fullBodyRect.top(), _fullBodyRect.width(), collapsedHeight);
        _endBodyRect = _fullBodyRect;
        _startOffsetY = _isOpenUp ? 12 : -12;
        _endOffsetY = 0;
        _revealAnimation->setDuration(180);
    }
    else
    {
        _startBodyRect = _comboTargetRect;
        _endBodyRect = _fullBodyRect;
        _startOffsetY = _getSelectedOffsetY(_comboTargetRect);
        _endOffsetY = 0;
        _revealAnimation->setDuration(200);
    }
    _revealAnimation->setStartValue(0.0);
    _revealAnimation->setEndValue(1.0);
    _applyAnimationFrame(0.0);
    _revealAnimation->start();
}

bool ElaComboBoxPopup::close()
{
    if (!_container || !_container->isVisible())
    {
        _resetAnimation();
        return false;
    }
    if (_isClosing)
    {
        return true;
    }
    if (!ElaColorAnimation::getIsAnimationEnabled(_comboBox) || !QApplication::isEffectEnabled(Qt::UI_AnimateCombo))
    {
        _resetAnimation();
        return false;
    }
    _revealAnimation->stop();
    _fullBodyRect = _container->rect().marginsRemoved(eTheme->getShadowMargins());
    _comboTargetRect = _getComboTargetRect();
    _startBodyRect = (_isAnimating && _bodyRect.isValid()) ? _bodyRect : _fullBodyRect;
    _startOffsetY = _isAnimating ? _contentOffsetY : 0;
    _startOpacity = _isAnimating ? _popupOpacity : 1.0;
    _startShadowOpacity = _isAnimating ? _shadowOpacity : 1.0;
    _startChromeOpacity = _isAnimating ? _itemChromeOpacity : 1.0;
    _isAnimating = true;
    _isClosing = true;
    if (_isDropDown)
    {
        const int collapsedHeight = qBound(16, qRound(_fullBodyRect.height() * 0.6), _fullBodyRect.height());
        _endBodyRect = _isOpenUp
            ? QRect(_fullBodyRect.x(), _fullBodyRect.bottom() - collapsedHeight + 1, _fullBodyRect.width(), collapsedHeight)
            : QRect(_fullBodyRect.x(), _fullBodyRect.top(), _fullBodyRect.width(), collapsedHeight);
        _endOffsetY = _isOpenUp ? 10 : -10;
        _revealAnimation->setEasingCurve(QEasingCurve::OutCubic);
        _revealAnimation->setDuration(150);
    }
    else
    {
        // 原地收回时收拢至组合框本体高度并同步平移新选中项对齐本体文字位置
        _endBodyRect = _comboTargetRect;
        _endOffsetY = _getSelectedOffsetY(_comboTargetRect);
        QEasingCurve closeEasing(QEasingCurve::BezierSpline);
        closeEasing.addCubicBezierSegment(QPointF(0, 0), QPointF(0, 1), QPointF(1, 1));
        _revealAnimation->setEasingCurve(closeEasing);
        _revealAnimation->setDuration(167);
    }
    _revealAnimation->setStartValue(0.0);
    _revealAnimation->setEndValue(1.0);
    _applyAnimationFrame(0.0);
    _revealAnimation->start();
    return true;
}

bool ElaComboBoxPopup::getIsVisible() const
{
    return _container && _container->isVisible() && !_isClosing;
}

bool ElaComboBoxPopup::getIsAnimating() const
{
    return _isAnimating;
}

bool ElaComboBoxPopup::getIsClosing() const
{
    return _isClosing;
}

bool ElaComboBoxPopup::getIsDropDown() const
{
    return _isDropDown;
}

bool ElaComboBoxPopup::getIsCoveringComboBox() const
{
    return !_isDropDown && _container && _container->isVisible();
}

QRect ElaComboBoxPopup::getBodyRect() const
{
    return _bodyRect;
}

qreal ElaComboBoxPopup::getPopupOpacity() const
{
    return _popupOpacity;
}

qreal ElaComboBoxPopup::getShadowOpacity() const
{
    return _shadowOpacity;
}

qreal ElaComboBoxPopup::getItemChromeOpacity() const
{
    return _itemChromeOpacity;
}

bool ElaComboBoxPopup::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == _container && event->type() == QEvent::Hide)
    {
        _resetAnimation();
        _comboBox->update();
    }
    else if (watched == _comboBox->view() && event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Escape || keyEvent->key() == Qt::Key_F4 || (keyEvent->key() == Qt::Key_Up && keyEvent->modifiers().testFlag(Qt::AltModifier)))
        {
            _comboBox->hidePopup();
            return true;
        }
    }
    return QObject::eventFilter(watched, event);
}

void ElaComboBoxPopup::_resetAnimation()
{
    _revealAnimation->stop();
    _isAnimating = false;
    _isClosing = false;
    _popupOpacity = 1.0;
    _shadowOpacity = 1.0;
    _itemChromeOpacity = 1.0;
    _contentOffsetY = 0;
    if (_container)
    {
        _container->clearMask();
        if (QLayout* layout = _container->layout())
        {
            layout->setEnabled(true);
            layout->activate();
        }
        _fullBodyRect = _container->rect().marginsRemoved(eTheme->getShadowMargins());
        _bodyRect = _fullBodyRect;
    }
    if (_comboBox && _comboBox->view())
    {
        QAbstractItemView* view = _comboBox->view();
        view->clearMask();
        if (auto* floatScrollBar = view->findChild<QScrollBar*>(QString(), Qt::FindDirectChildrenOnly))
        {
            floatScrollBar->setVisible(floatScrollBar->maximum() > 0);
        }
        view->viewport()->update();
    }
    if (_container)
    {
        _container->update();
    }
}

void ElaComboBoxPopup::_applyAnimationFrame(qreal progress)
{
    if (!_container)
    {
        return;
    }
    const int bodyTop = qRound(_startBodyRect.top() + (_endBodyRect.top() - _startBodyRect.top()) * progress);
    const int bodyHeight = qMax(1, qRound(_startBodyRect.height() + (_endBodyRect.height() - _startBodyRect.height()) * progress));
    _bodyRect = QRect(_fullBodyRect.x(), bodyTop, _fullBodyRect.width(), bodyHeight);
    _contentOffsetY = qRound(_startOffsetY + (_endOffsetY - _startOffsetY) * progress);
    if (_isDropDown)
    {
        if (_isClosing)
        {
            _popupOpacity = qBound(0.0, _startOpacity * (1.0 - progress), 1.0);
            _shadowOpacity = qBound(0.0, _startShadowOpacity * (1.0 - progress), 1.0);
        }
        else
        {
            const qreal fadeIn = _revealAnimation->duration() > 0 ? qBound(0.0, _revealAnimation->currentTime() / 83.0, 1.0) : 1.0;
            _popupOpacity = qMax(0.25, fadeIn);
            _shadowOpacity = _popupOpacity * progress;
        }
        _itemChromeOpacity = 1.0;
    }
    else
    {
        _popupOpacity = 1.0;
        if (_isClosing)
        {
            _shadowOpacity = qBound(0.0, _startShadowOpacity * (1.0 - progress), 1.0);
            _itemChromeOpacity = qBound(0.0, _startChromeOpacity * (1.0 - progress), 1.0);
        }
        else
        {
            _shadowOpacity = qBound(0.0, progress, 1.0);
            _itemChromeOpacity = qBound(0.0, progress, 1.0);
        }
    }
    if (QLayout* layout = _container->layout())
    {
        layout->setEnabled(false);
    }
    QAbstractItemView* view = _comboBox->view();
    view->setGeometry(_fullBodyRect.x(), _fullBodyRect.y() + _contentOffsetY, _fullBodyRect.width(), _fullBodyRect.height());
    if (auto* floatScrollBar = view->findChild<QScrollBar*>(QString(), Qt::FindDirectChildrenOnly))
    {
        floatScrollBar->setVisible(false);
    }
    const QRect clipInContainer = _bodyRect.adjusted(1, 2, -1, -2);
    const QRect clipInView = clipInContainer.translated(-view->pos()).intersected(view->rect());
    if (clipInView.isEmpty())
    {
        view->setMask(QRegion(-10, -10, 1, 1));
    }
    else
    {
        view->setMask(QRegion(clipInView));
    }
    _container->update();
    _comboBox->update();
}

QRect ElaComboBoxPopup::_getComboTargetRect() const
{
    if (!_container || !_comboBox)
    {
        return _fullBodyRect;
    }
    const QPoint topLeftInContainer = _container->mapFromGlobal(_comboBox->mapToGlobal(QPoint(0, 0)));
    const int targetHeight = qBound(1, _comboBox->height(), _fullBodyRect.height());
    const int targetTop = qBound(_fullBodyRect.top(), topLeftInContainer.y(), _fullBodyRect.bottom() - targetHeight + 1);
    return QRect(_fullBodyRect.x(), targetTop, _fullBodyRect.width(), targetHeight);
}

int ElaComboBoxPopup::_getSelectedOffsetY(const QRect& targetRect) const
{
    if (!_comboBox || !_container)
    {
        return 0;
    }
    const int selectedRow = _comboBox->currentIndex();
    if (selectedRow < 0 || selectedRow >= _comboBox->count())
    {
        return 0;
    }
    QAbstractItemView* view = _comboBox->view();
    const QModelIndex selectedIndex = _comboBox->model()->index(selectedRow, _comboBox->modelColumn(), _comboBox->rootModelIndex());
    const QRect itemRectInViewport = view->visualRect(selectedIndex);
    if (!itemRectInViewport.isValid())
    {
        return 0;
    }
    const QRect viewportProbe = view->viewport()->rect().adjusted(0, -itemRectInViewport.height(), 0, itemRectInViewport.height());
    if (!viewportProbe.intersects(itemRectInViewport))
    {
        return 0;
    }
    const qreal itemCenterInContainerY = _fullBodyRect.top() + view->viewport()->y() + QRectF(itemRectInViewport).center().y();
    return qRound(QRectF(targetRect).center().y() - itemCenterInContainerY);
}
