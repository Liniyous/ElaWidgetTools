#include "ElaNavigationView.h"

#include "ElaColorAnimation.h"
#include "ElaMenu.h"
#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationStyle.h"
#include "ElaScrollBar.h"
#include "ElaTheme.h"
#include "ElaToolTip.h"

#include <QDebug>
#include <QHeaderView>
#include <QModelIndex>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QScrollBar>
#include <QScroller>
#include <QVariantAnimation>

ElaNavigationView::ElaNavigationView(QWidget* parent)
    : QTreeView(parent)
{
    setObjectName("ElaNavigationView");
    setStyleSheet("#ElaNavigationView{background-color:transparent;}");
    setAnimated(true);
    setHeaderHidden(true);
    setRootIsDecorated(false);
    setItemsExpandable(false);
    setExpandsOnDoubleClick(false);
    setAutoScroll(false);
    setMouseTracking(true);
    setSelectionMode(QAbstractItemView::NoSelection);

    // 滚动条设置
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ElaScrollBar* vScrollBar = new ElaScrollBar(this);
    connect(vScrollBar, &ElaScrollBar::rangeAnimationFinished, this, [=]() {
        doItemsLayout();
    });
    setVerticalScrollBar(vScrollBar);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ElaScrollBar* floatVScrollBar = new ElaScrollBar(vScrollBar, this);
    floatVScrollBar->setIsAnimation(true);
    floatVScrollBar->installEventFilter(this);

    _navigationStyle = new ElaNavigationStyle(this);
    setStyle(_navigationStyle);
    _expandAnimation = new QVariantAnimation(this);
    _expandAnimation->setDuration(200);
    QEasingCurve easing(QEasingCurve::BezierSpline);
    easing.addCubicBezierSegment(QPointF(0, 0.35), QPointF(0.15, 1), QPointF(1, 1));
    _expandAnimation->setEasingCurve(easing);
    connect(_expandAnimation, &QVariantAnimation::valueChanged, viewport(), QOverload<>::of(&QWidget::update));
    connect(_expandAnimation, &QVariantAnimation::finished, this, &ElaNavigationView::_finishExpandAnimation);
    connect(vScrollBar, &QScrollBar::valueChanged, this, &ElaNavigationView::_finishExpandAnimation);
    connect(eTheme, &ElaTheme::themeModeChanged, this, &ElaNavigationView::_finishExpandAnimation);

    QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller* scroller = QScroller::scroller(this->viewport());
    QScrollerProperties properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    scroller->setScrollerProperties(properties);

    connect(scroller, &QScroller::stateChanged, this, [=](QScroller::State newstate) {
        if (newstate == QScroller::Pressed)
        {
            _navigationStyle->setPressIndex(indexAt(mapFromGlobal(QCursor::pos())));
            viewport()->update();
        }
        else if (newstate == QScroller::Scrolling || newstate == QScroller::Inactive)
        {
            _navigationStyle->setPressIndex(QModelIndex());
        }
    });

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &ElaNavigationView::customContextMenuRequested, this, &ElaNavigationView::onCustomContextMenuRequested);
}

ElaNavigationView::~ElaNavigationView()
{
}

void ElaNavigationView::setModel(QAbstractItemModel* model)
{
    _finishExpandAnimation();
    QTreeView::setModel(model);
    if (model)
    {
        connect(model, &QAbstractItemModel::rowsAboutToBeRemoved, this, &ElaNavigationView::_finishExpandAnimation);
        connect(model, &QAbstractItemModel::rowsAboutToBeInserted, this, &ElaNavigationView::_finishExpandAnimation);
        connect(model, &QAbstractItemModel::modelAboutToBeReset, this, &ElaNavigationView::_finishExpandAnimation);
        connect(model, &QAbstractItemModel::dataChanged, this, &ElaNavigationView::_finishExpandAnimation);
    }
}

void ElaNavigationView::setNodeExpanded(ElaNavigationNode* node, bool isExpanded)
{
    const QModelIndex index = node->getModelIndex();
    const int top = visualRect(index).bottom() + 1;
    const bool isAnimationEnabled = isAnimated() && isVisible() && top > 0 && top < viewport()->height()
        && ElaColorAnimation::getIsAnimationEnabled(viewport());
    const bool isReversing = isAnimationEnabled && _expandAnimation->state() == QAbstractAnimation::Running && _expandIndex == index;
    const qreal startHeight = isReversing ? _expandAnimation->currentValue().toReal() : isExpanded ? 0
                                                                                                   : _getExpandedHeight(index);
    if (isReversing)
    {
        _expandAnimation->stop();
    }
    else
    {
        _finishExpandAnimation();
    }
    QPixmap before;
    if (isAnimationEnabled && !isReversing)
    {
        before = viewport()->grab();
    }
    // 使用公开接口提交展开状态 避免叠加 Qt 内部不可配置的树动画
    const bool wasAnimated = isAnimated();
    setAnimated(false);
    setExpanded(index, isExpanded);
    doItemsLayout();
    setAnimated(wasAnimated);
    _navigationStyle->startExpandAnimation(node, isExpanded);
    if (!isAnimationEnabled || (isReversing && (_collapsedPixmap.isNull() || _expandedPixmap.isNull())))
    {
        return;
    }
    if (!isReversing)
    {
        _expandTop = top;
        _expandHeight = isExpanded ? _getExpandedHeight(index) : qRound(startHeight);
        _collapsedPixmap = isExpanded ? before : viewport()->grab();
        _expandedPixmap = isExpanded ? viewport()->grab() : before;
    }
    _expandIndex = index;
    _expandAnimation->setStartValue(startHeight);
    _expandAnimation->setEndValue(isExpanded ? static_cast<qreal>(_expandHeight) : 0.0);
    _expandAnimation->start();
}

void ElaNavigationView::startSelectionAnimation(ElaNavigationNode* previousNode, ElaNavigationNode* selectedNode)
{
    _navigationStyle->startSelectionAnimation(previousNode, selectedNode);
}

void ElaNavigationView::paintEvent(QPaintEvent* event)
{
    if (!_expandAnimation || _expandAnimation->state() != QAbstractAnimation::Running || !_expandIndex.isValid())
    {
        QTreeView::paintEvent(event);
        return;
    }
    QPaintEvent headerEvent(event->region().intersected(QRect(0, 0, viewport()->width(), _expandTop)));
    QTreeView::paintEvent(&headerEvent);
    const int height = qRound(_expandAnimation->currentValue().toReal());
    QPainter painter(viewport());
    painter.setClipRect(QRect(0, _expandTop, viewport()->width(), height));
    painter.drawPixmap(0, height - _expandHeight, _expandedPixmap);
    painter.setClipRect(QRect(0, _expandTop + height, viewport()->width(), viewport()->height()));
    painter.drawPixmap(0, height, _collapsedPixmap);
}

void ElaNavigationView::resizeEvent(QResizeEvent* event)
{
    _finishExpandAnimation();
    QTreeView::resizeEvent(event);
}

void ElaNavigationView::onCustomContextMenuRequested(const QPoint& pos)
{
    if (!_pNavigationBarPrivate->_pIsAllowPageOpenInNewWindow)
    {
        return;
    }
    QModelIndex posIndex = indexAt(pos);
    if (!posIndex.isValid())
    {
        return;
    }
    ElaNavigationNode* posNode = static_cast<ElaNavigationNode*>(posIndex.internalPointer());
    if (!posNode->getIsExpanderNode() && !posNode->getIsCategoryNode())
    {
        ElaMenu menu;
        menu.setMenuItemHeight(27);
        QAction* openAction = menu.addElaIconAction(ElaIconType::ObjectGroup, "在新窗口中打开");
        connect(openAction, &QAction::triggered, this, [=]() {
            Q_EMIT navigationOpenNewWindow(posNode->getNodeKey());
        });
        menu.exec(mapToGlobal(pos));
    }
}

void ElaNavigationView::mouseMoveEvent(QMouseEvent* event)
{
    _doCompactToolTip();
    QTreeView::mouseMoveEvent(event);
}

void ElaNavigationView::mouseDoubleClickEvent(QMouseEvent* event)
{
    _navigationStyle->setPressIndex(indexAt(event->pos()));
    viewport()->update();
    QTreeView::mouseDoubleClickEvent(event);
}

void ElaNavigationView::mouseReleaseEvent(QMouseEvent* event)
{
    QTreeView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid() && (_expandAnimation->state() != QAbstractAnimation::Running || event->pos().y() < _expandTop))
        {
            Q_EMIT navigationClicked(index);
        }
        _navigationStyle->setPressIndex(QModelIndex());
    }
}

bool ElaNavigationView::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::MouseMove:
    case QEvent::HoverMove:
    {
        _doCompactToolTip();
        break;
    }
    default:
    {
        break;
    }
    }
    return QAbstractItemView::eventFilter(watched, event);
}

int ElaNavigationView::_getExpandedHeight(const QModelIndex& index) const
{
    const int top = visualRect(index).bottom() + 1;
    int height = 0;
    for (auto child = indexBelow(index); child.isValid(); child = indexBelow(child))
    {
        auto parent = child.parent();
        while (parent.isValid() && parent != index)
        {
            parent = parent.parent();
        }
        if (parent != index)
        {
            break;
        }
        height = visualRect(child).bottom() + 1 - top;
        if (height >= viewport()->height() - top)
        {
            return viewport()->height() - top;
        }
    }
    return height;
}

void ElaNavigationView::_finishExpandAnimation()
{
    if (_expandAnimation)
    {
        _expandAnimation->stop();
    }
    _expandIndex = QPersistentModelIndex();
    _collapsedPixmap = QPixmap();
    _expandedPixmap = QPixmap();
    viewport()->update();
}

void ElaNavigationView::_doCompactToolTip()
{
    if (_pNavigationBarPrivate->_currentDisplayMode == ElaNavigationType::NavigationDisplayMode::Compact)
    {
        if (!_compactToolTip)
        {
            _compactToolTip = new ElaToolTip(this);
        }
        QModelIndex posIndex = indexAt(mapFromGlobal(QCursor::pos()));
        if (!posIndex.isValid())
        {
            _compactToolTip->hide();
            return;
        }
        ElaNavigationNode* posNode = static_cast<ElaNavigationNode*>(posIndex.internalPointer());
        _compactToolTip->setToolTip(posNode->getNodeTitle());
        _compactToolTip->updatePos();
        _compactToolTip->show();
    }
    else
    {
        if (_compactToolTip)
        {
            _compactToolTip->deleteLater();
            _compactToolTip = nullptr;
        }
    }
}
