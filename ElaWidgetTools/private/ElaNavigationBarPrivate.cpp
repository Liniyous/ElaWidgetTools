#include "ElaNavigationBarPrivate.h"

#include "ElaActionCommander.h"
#include "ElaApplication.h"
#include "ElaBaseListView.h"
#include "ElaColorAnimation.h"
#include "ElaCustomTabWidget.h"
#include "ElaCustomWidget.h"
#include "ElaFooterDelegate.h"
#include "ElaFooterModel.h"
#include "ElaIconButton.h"
#include "ElaInteractiveCard.h"
#include "ElaMenu.h"
#include "ElaNavigationBar.h"
#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationRouteCommand.h"
#include "ElaNavigationView.h"
#include "ElaTabWidget.h"
#include "ElaToolButton.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QLayout>
#include <QPropertyAnimation>
#include <QScrollBar>
#include <QSignalBlocker>
#include <QTimer>
#include <QVariantAnimation>

ElaNavigationBarPrivate::ElaNavigationBarPrivate(QObject* parent)
    : QObject{parent}
{
    _scrollTimer = new QTimer(this);
    _scrollTimer->setSingleShot(true);
    _scrollTimer->setInterval(200);
    connect(_scrollTimer, &QTimer::timeout, this, [this]() {
        _scrollToPendingNode();
    });
}

ElaNavigationBarPrivate::~ElaNavigationBarPrivate()
{
}

void ElaNavigationBarPrivate::onNavigationOpenNewWindow(const QString& nodeKey)
{
    Q_Q(ElaNavigationBar);
    const QMetaObject* meta = _pageMetaMap.value(nodeKey);
    if (!meta)
    {
        return;
    }
    QWidget* widget = dynamic_cast<QWidget*>(meta->newInstance());
    if (widget)
    {
        widget->setProperty("ElaPageKey", nodeKey);
        widget->setProperty("IsMetaWidget", true);
        widget->setProperty("ElaFloatParentWidget", QVariant::fromValue(q));
        widget->installEventFilter(this);
        ElaCustomTabWidget* floatWidget = new ElaCustomTabWidget(q);
        floatWidget->getCustomTabWidget()->addTab(widget, widget->windowIcon(), widget->windowTitle());
        floatWidget->show();
        Q_EMIT q->pageOpenInNewWindow(nodeKey);
    }
}

void ElaNavigationBarPrivate::onTreeViewClicked(const QModelIndex& index, bool isLogRoute, bool isRouteBack, bool isScrollToNode)
{
    Q_Q(ElaNavigationBar);
    if (!index.isValid())
    {
        return;
    }
    ElaNavigationNode* node = static_cast<ElaNavigationNode*>(index.internalPointer());
    if (node->getIsCategoryNode())
    {
        Q_EMIT q->navigationNodeClicked(ElaNavigationType::CategoryNode, node->getNodeKey(), isRouteBack);
    }
    else if (node->getIsExpanderNode())
    {
        _expandOrCollapseExpanderNode(node, !_navigationView->isExpanded(index));
        _resetNodeSelected();
    }
    else
    {
        _activatePage(node, isLogRoute, isRouteBack, isScrollToNode);
    }
}

void ElaNavigationBarPrivate::onFooterViewClicked(const QModelIndex& index, bool isLogRoute, bool isRouteBack)
{
    if (index.isValid())
    {
        _activatePage(index.data(Qt::UserRole).value<ElaNavigationNode*>(), isLogRoute, isRouteBack, false);
    }
}

void ElaNavigationBarPrivate::_activatePage(ElaNavigationNode* node, bool isLogRoute, bool isRouteBack, bool isScrollToNode)
{
    Q_Q(ElaNavigationBar);
    if (!node)
    {
        return;
    }
    const bool isFooter = node->getIsFooterNode();
    const bool hasPage = !isFooter || node->getIsHasFooterPage();
    if (node->getKeyPoints())
    {
        node->setKeyPoints(0);
        (isFooter ? _footerView->viewport() : _navigationView->viewport())->update();
    }
    ElaNavigationNode* previousNode = _getSelectedNode();
    if (hasPage && previousNode != node)
    {
        _smoothScrollNavigationView(QModelIndex());
    }
    if (!isFooter)
    {
        _ensureNodeVisible(node);
    }
    if (hasPage)
    {
        _setSelectedNode(node);
    }
    if (isScrollToNode)
    {
        _smoothScrollNavigationView(node->getModelIndex());
    }
    if (previousNode == node)
    {
        return;
    }
    const QString nodeKey = node->getNodeKey();
    if (isLogRoute && hasPage)
    {
        auto command = new ElaNavigationRouteCommand(this);
        command->setNavigationBar(q);
        command->setUndoPageKey(previousNode ? previousNode->getNodeKey() : QString());
        command->setRedoPageKey(nodeKey);
        ElaActionCommander::getInstance()->recordCommand("ElaWidgetToolsAction", command, false);
    }
    // 命令状态通知也可能触发新导航，外层请求只在仍为当前页面时继续通知
    if (!hasPage || _getSelectedNode() == node)
    {
        Q_EMIT q->navigationNodeClicked(isFooter ? ElaNavigationType::FooterNode : ElaNavigationType::PageNode, nodeKey, isRouteBack);
    }
}

ElaNavigationNode* ElaNavigationBarPrivate::_getSelectedNode() const
{
    return _navigationModel->getSelectedNode() ? _navigationModel->getSelectedNode() : _footerModel->getSelectedNode();
}

ElaNavigationNode* ElaNavigationBarPrivate::_getVisibleNode(ElaNavigationNode* node) const
{
    if (!node || node->getIsFooterNode())
    {
        return node;
    }
    if (_currentDisplayMode == ElaNavigationType::Compact)
    {
        return node->getOriginalNode();
    }
    while (!node->getIsVisible() && !node->getParentNode()->getIsRootNode())
    {
        node = node->getParentNode();
    }
    return node;
}

void ElaNavigationBarPrivate::_setSelectedNode(ElaNavigationNode* node)
{
    ElaNavigationNode* previousMainNode = _navigationModel->getSelectedExpandedNode() ? _navigationModel->getSelectedExpandedNode() : _navigationModel->getSelectedNode();
    ElaNavigationNode* previousFooterNode = _footerModel->getSelectedNode();
    ElaNavigationNode* mainNode = node && !node->getIsFooterNode() ? node : nullptr;
    ElaNavigationNode* footerNode = node && node->getIsFooterNode() ? node : nullptr;
    ElaNavigationNode* visibleNode = _getVisibleNode(mainNode);
    _navigationModel->setSelectedNode(mainNode);
    _navigationModel->setSelectedExpandedNode(visibleNode != mainNode ? visibleNode : nullptr);
    _footerModel->setSelectedNode(footerNode);
    _updateSelection(_navigationView, visibleNode);
    _updateSelection(_footerView, footerNode);
    _startSelectionAnimation(previousMainNode, visibleNode, false);
    _startSelectionAnimation(previousFooterNode, footerNode, true);
}

void ElaNavigationBarPrivate::_updateSelection(QAbstractItemView* view, ElaNavigationNode* node)
{
    const QModelIndex index = node ? node->getModelIndex() : QModelIndex();
    if (index.isValid())
    {
        if (!view->selectionModel()->isSelected(index))
        {
            view->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
        }
    }
    else
    {
        view->clearSelection();
    }
}

void ElaNavigationBarPrivate::_startSelectionAnimation(ElaNavigationNode* previousNode, ElaNavigationNode* selectedNode, bool isFooter)
{
    if (previousNode != selectedNode)
    {
        if (isFooter)
        {
            _footerDelegate->startSelectionAnimation(previousNode, selectedNode);
        }
        else
        {
            _navigationView->startSelectionAnimation(previousNode, selectedNode);
        }
    }
}

bool ElaNavigationBarPrivate::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Show:
    {
        QString nodeKey = watched->property("ElaPageKey").toString();
        if (!nodeKey.isNull())
        {
            _pageNewWindowCountMap[nodeKey] += 1;
        }
        break;
    }
    case QEvent::HideToParent:
    {
        QString nodeKey = watched->property("ElaPageKey").toString();
        if (!nodeKey.isNull())
        {
            _pageNewWindowCountMap[nodeKey] -= 1;
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return QObject::eventFilter(watched, event);
}

void ElaNavigationBarPrivate::_initNodeModelIndex(const QModelIndex& parentIndex)
{
    int rowCount = _navigationModel->rowCount(parentIndex);
    for (int row = 0; row < rowCount; ++row)
    {
        QModelIndex index = _navigationModel->index(row, 0, parentIndex);
        ElaNavigationNode* childNode = static_cast<ElaNavigationNode*>(index.internalPointer());
        childNode->setModelIndex(index);
        if (!parentIndex.isValid() && childNode->getIsCategoryNode())
        {
            _navigationView->setRowHidden(row, parentIndex, _currentDisplayMode != ElaNavigationType::Maximal);
        }
        if (_navigationModel->hasChildren(index))
        {
            _initNodeModelIndex(index);
        }
    }
}

void ElaNavigationBarPrivate::_resetNodeSelected()
{
    _setSelectedNode(_getSelectedNode());
}

void ElaNavigationBarPrivate::_ensureNodeVisible(ElaNavigationNode* node)
{
    if (_currentDisplayMode == ElaNavigationType::Compact)
    {
        return;
    }
    QList<ElaNavigationNode*> ancestors;
    for (ElaNavigationNode* parent = node->getParentNode(); parent && !parent->getIsRootNode(); parent = parent->getParentNode())
    {
        ancestors.prepend(parent);
    }
    for (auto ancestor: ancestors)
    {
        _setNodeExpanded(ancestor, true);
    }
}

void ElaNavigationBarPrivate::_setNodeExpanded(ElaNavigationNode* node, bool isExpanded)
{
    if (!node || !node->getIsHasChild() || _navigationView->isExpanded(node->getModelIndex()) == isExpanded)
    {
        return;
    }
    node->setIsExpanded(isExpanded);
    _navigationView->setNodeExpanded(node, isExpanded);
}

void ElaNavigationBarPrivate::_expandOrCollapseExpanderNode(ElaNavigationNode* node, bool isExpand)
{
    if (_currentDisplayMode == ElaNavigationType::Compact)
    {
        if (node->getIsHasPageChild())
        {
            //展开菜单
            ElaMenu* menu = _compactMenuMap.value(node);
            if (menu)
            {
                QPoint nodeTopRight = _navigationView->mapToGlobal(_navigationView->visualRect(node->getModelIndex()).topRight());
                menu->popup(QPoint(nodeTopRight.x() + 10, nodeTopRight.y()));
            }
        }
    }
    else
    {
        _setNodeExpanded(node, isExpand);
    }
}

void ElaNavigationBarPrivate::_addStackedPage(QWidget* page, QString pageKey)
{
    Q_Q(ElaNavigationBar);
    if (_footerModel->getSelectedNode())
    {
        _navigationModel->setSelectedNode(nullptr);
    }
    page->setProperty("ElaPageKey", pageKey);
    Q_EMIT q->navigationNodeAdded(ElaNavigationType::PageNode, pageKey, page);
    ElaNavigationNode* node = _navigationModel->getNavigationNode(pageKey);
    if (!node->getParentNode()->getIsRootNode())
    {
        ElaNavigationNode* originalNode = node->getOriginalNode();
        ElaMenu*& menu = _compactMenuMap[originalNode];
        if (!menu)
        {
            menu = new ElaMenu(q);
        }
        QAction* action = menu->addElaIconAction(node->getAwesome(), node->getNodeTitle());
        action->setData(pageKey);
        connect(action, &QAction::triggered, this, [this, pageKey]() {
            if (auto targetNode = _navigationModel->getNavigationNode(pageKey))
            {
                onTreeViewClicked(targetNode->getModelIndex());
            }
        });
    }
    QVariantMap suggestData;
    suggestData.insert("ElaPageKey", pageKey);
    _suggestDataList.append(ElaSuggestBox::SuggestData(node->getAwesome(), node->getNodeTitle(), suggestData));
}

void ElaNavigationBarPrivate::_addFooterPage(QWidget* page, QString footKey)
{
    Q_Q(ElaNavigationBar);
    Q_EMIT q->navigationNodeAdded(ElaNavigationType::FooterNode, footKey, page);
    if (page)
    {
        page->setProperty("ElaPageKey", footKey);
    }
    _footerView->setFixedHeight(ElaWidgetToolsSize::DefaultListItemHeight * _footerModel->getFooterNodeCount());
    ElaNavigationNode* node = _footerModel->getNavigationNode(footKey);
    QVariantMap suggestData;
    suggestData.insert("ElaPageKey", footKey);
    _suggestDataList.append(ElaSuggestBox::SuggestData(node->getAwesome(), node->getNodeTitle(), suggestData));
}

void ElaNavigationBarPrivate::_raiseNavigationBar()
{
    Q_Q(ElaNavigationBar);
    q->raise();
}

void ElaNavigationBarPrivate::_smoothScrollNavigationView(const QModelIndex& index)
{
    _scrollTimer->stop();
    if (_scrollAnimation)
    {
        _scrollAnimation->stop();
    }
    _pendingScrollIndex = index;
    if (index.isValid() && _currentDisplayMode != ElaNavigationType::Compact)
    {
        _scrollTimer->start();
    }
}

void ElaNavigationBarPrivate::_scrollToPendingNode()
{
    const QModelIndex index = _pendingScrollIndex;
    _pendingScrollIndex = QModelIndex();
    ElaNavigationNode* selectedNode = _navigationModel->getSelectedNode();
    if (!index.isValid() || _currentDisplayMode == ElaNavigationType::Compact || !selectedNode || selectedNode->getModelIndex() != index)
    {
        return;
    }
    auto scrollBar = _navigationView->verticalScrollBar();
    const int startValue = scrollBar->value();
    int endValue = startValue;
    {
        // 用 Qt 计算居中位置，测量期间不发送中间滚动位置
        QSignalBlocker blocker(scrollBar);
        _navigationView->scrollTo(index, QAbstractItemView::PositionAtCenter);
        endValue = scrollBar->value();
        scrollBar->setValue(startValue);
    }
    if (startValue == endValue)
    {
        return;
    }
    if (!_scrollAnimation)
    {
        _scrollAnimation = new QPropertyAnimation(scrollBar, "value", this);
        _scrollAnimation->setEasingCurve(QEasingCurve::OutSine);
        _scrollAnimation->setDuration(255);
    }
    _scrollAnimation->setStartValue(startValue);
    _scrollAnimation->setEndValue(endValue);
    _scrollAnimation->start();
}

void ElaNavigationBarPrivate::_doComponentAnimation(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation)
{
    const bool wasMaximal = _currentDisplayMode == ElaNavigationType::Maximal;
    const bool isMaximal = displayMode == ElaNavigationType::Maximal;
    _currentDisplayMode = displayMode;
    _updateCategoryVisibility();
    _updateDisplayLayout();
    if (!isMaximal)
    {
        _smoothScrollNavigationView(QModelIndex());
    }
    if (wasMaximal != isMaximal)
    {
        _handleNavigationExpandState(!isMaximal, isAnimation);
    }
    _doNavigationBarWidthAnimation(displayMode, isAnimation);
}

void ElaNavigationBarPrivate::_handleNavigationExpandState(bool isSave, bool isAnimation)
{
    const bool isAnimated = _navigationView->isAnimated();
    _navigationView->setAnimated(isAnimated && !isSave && isAnimation);
    if (isSave)
    {
        _lastExpandedNodeKeys.clear();
        for (auto node: _navigationModel->getRootExpandedNodes())
        {
            _lastExpandedNodeKeys.append(node->getNodeKey());
            _setNodeExpanded(node, false);
        }
    }
    else
    {
        for (const auto& nodeKey: _lastExpandedNodeKeys)
        {
            _setNodeExpanded(_navigationModel->getNavigationNode(nodeKey), true);
        }
        _lastExpandedNodeKeys.clear();
    }
    _navigationView->setAnimated(isAnimated);
    _resetNodeSelected();
}

void ElaNavigationBarPrivate::_updateCategoryVisibility()
{
    for (auto node: _navigationModel->getRootCategoryNodes())
    {
        _navigationView->setRowHidden(node->getRow(), QModelIndex(), _currentDisplayMode != ElaNavigationType::Maximal);
    }
}

void ElaNavigationBarPrivate::_updateDisplayLayout()
{
    Q_Q(ElaNavigationBar);
    const bool isMaximal = _currentDisplayMode == ElaNavigationType::Maximal;
    const int contentWidth = isMaximal ? _pNavigationBarWidth : 42;
    _navigationView->setFixedWidth(contentWidth);
    _navigationView->setColumnWidth(0, contentWidth - 2 * _navigationView->frameWidth());
    _footerView->setFixedWidth(contentWidth);
    _userCard->setFixedWidth(_pNavigationBarWidth - 3);
    _userCard->setVisible(_isShowUserCard && isMaximal);
    _userButton->setVisible(_isShowUserCard && !isMaximal);
    _userCardLayout->setContentsMargins(_isShowUserCard ? QMargins(3, 10, 0, 5) : QMargins());
    q->layout()->activate();
}

void ElaNavigationBarPrivate::_doNavigationBarWidthAnimation(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation)
{
    Q_Q(ElaNavigationBar);
    if (!_navigationBarWidthAnimation)
    {
        _navigationBarWidthAnimation = new QVariantAnimation(this);
        // 对齐 WinUI SplitView 的紧凑内联过渡
        QEasingCurve easing(QEasingCurve::BezierSpline);
        easing.addCubicBezierSegment(QPointF(0, 0.35), QPointF(0.15, 1), QPointF(1, 1));
        _navigationBarWidthAnimation->setEasingCurve(easing);
        connect(_navigationBarWidthAnimation, &QVariantAnimation::valueChanged, this, [q](const QVariant& value) {
            q->setFixedWidth(value.toInt());
        });
    }
    const int targetWidth = displayMode == ElaNavigationType::Minimal ? 0 : displayMode == ElaNavigationType::Compact ? 42
                                                                                                                      : _pNavigationBarWidth;
    _navigationBarWidthAnimation->stop();
    _navigationBarWidthAnimation->setStartValue(q->width());
    _navigationBarWidthAnimation->setEndValue(targetWidth);
    _navigationBarWidthAnimation->setDuration(isAnimation && ElaColorAnimation::getIsAnimationEnabled(q) ? 200 : 0);
    _navigationBarWidthAnimation->start();
}
