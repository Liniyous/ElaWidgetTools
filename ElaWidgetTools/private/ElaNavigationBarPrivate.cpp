#include "ElaNavigationBarPrivate.h"
#include "ElaApplication.h"
#include "ElaBaseListView.h"
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
#include "ElaNavigationRouter.h"
#include "ElaNavigationView.h"
#include "ElaToolButton.h"
#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QLayout>
#include <QPropertyAnimation>
#include <QScrollBar>
#include <QTimer>
ElaNavigationBarPrivate::ElaNavigationBarPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaNavigationBarPrivate::~ElaNavigationBarPrivate()
{
}

void ElaNavigationBarPrivate::onNavigationOpenNewWindow(QString nodeKey)
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
        floatWidget->addTab(widget, widget->windowIcon(), widget->windowTitle());
        floatWidget->show();
        Q_EMIT q->pageOpenInNewWindow(nodeKey);
    }
}

void ElaNavigationBarPrivate::onNavigationRoute(QVariantMap routeData)
{
    Q_Q(ElaNavigationBar);
    bool isRouteBack = routeData.value("ElaRouteBackMode").toBool();
    QString pageKey = isRouteBack ? routeData.value("ElaBackPageKey").toString() : routeData.value("ElaForwardPageKey").toString();
    q->navigation(pageKey, false, isRouteBack);
}

void ElaNavigationBarPrivate::onTreeViewClicked(const QModelIndex& index, bool isLogRoute, bool isRouteBack)
{
    Q_Q(ElaNavigationBar);
    if (index.isValid())
    {
        ElaNavigationNode* node = static_cast<ElaNavigationNode*>(index.internalPointer());
        if (!node)
        {
            return;
        }
        if (node->getIsExpanderNode())
        {
            _expandOrCollapseExpanderNode(node, !_navigationView->isExpanded(index));
        }
        else
        {
            if (node->getKeyPoints())
            {
                node->setKeyPoints(0);
                _navigationView->update();
            }
            ElaNavigationNode* selectedNode = _navigationModel->getSelectedNode();
            if (selectedNode != node)
            {
                // 记录跳转
                if (isLogRoute)
                {
                    QVariantMap routeData = QVariantMap();
                    QString backPageKey;
                    if (selectedNode)
                    {
                        backPageKey = selectedNode->getNodeKey();
                    }
                    else
                    {
                        if (_footerModel->getSelectedNode())
                        {
                            backPageKey = _footerModel->getSelectedNode()->getNodeKey();
                        }
                    }
                    routeData.insert("ElaBackPageKey", backPageKey);
                    routeData.insert("ElaForwardPageKey", node->getNodeKey());
                    ElaNavigationRouter::getInstance()->navigationRoute(this, "onNavigationRoute", routeData);
                }
                Q_EMIT q->navigationNodeClicked(ElaNavigationType::PageNode, node->getNodeKey(), isRouteBack);

                if (_footerModel->getSelectedNode())
                {
                    _footerView->clearSelection();
                    QVariantMap footerPostData = QVariantMap();
                    footerPostData.insert("SelectMarkChanged", true);
                    footerPostData.insert("LastSelectedNode", QVariant::fromValue(_footerModel->getSelectedNode()));
                    footerPostData.insert("SelectedNode", QVariant::fromValue(nullptr));
                    _footerModel->setSelectedNode(nullptr);
                    _footerDelegate->navigationNodeStateChange(footerPostData);
                }
                QVariantMap postData = QVariantMap();
                postData.insert("SelectMarkChanged", true);
                if (_navigationModel->getSelectedExpandedNode())
                {
                    postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedExpandedNode()));
                }
                else
                {
                    postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedNode()));
                }
                if (_currentDisplayMode == ElaNavigationType::Compact)
                {
                    ElaNavigationNode* originNode = node->getOriginalNode();
                    if (originNode == node)
                    {
                        postData.insert("SelectedNode", QVariant::fromValue(node));
                    }
                    else
                    {
                        if (originNode == _navigationModel->getSelectedExpandedNode())
                        {
                            _navigationModel->setSelectedNode(node);
                            _resetNodeSelected();
                            return;
                        }
                        _navigationModel->setSelectedExpandedNode(originNode);
                        postData.insert("SelectedNode", QVariant::fromValue(originNode));
                    }
                }
                else
                {
                    postData.insert("SelectedNode", QVariant::fromValue(node));
                }
                _navigationModel->setSelectedNode(node);
                _navigationView->navigationNodeStateChange(postData);
                if (!node->getIsVisible() && _currentDisplayMode != ElaNavigationType::Compact)
                {
                    _expandSelectedNodeParent();
                }
            }
        }
        _resetNodeSelected();
    }
}

void ElaNavigationBarPrivate::onFooterViewClicked(const QModelIndex& index, bool isLogRoute, bool isRouteBack)
{
    Q_Q(ElaNavigationBar);
    ElaNavigationNode* node = index.data(Qt::UserRole).value<ElaNavigationNode*>();
    if (node->getKeyPoints())
    {
        node->setKeyPoints(0);
        _navigationView->update();
    }
    ElaNavigationNode* selectedNode = _footerModel->getSelectedNode();

    if (selectedNode != node)
    {
        // 记录跳转
        if (isLogRoute && node->getIsHasFooterPage())
        {
            QVariantMap routeData = QVariantMap();
            QString backPageKey;
            if (selectedNode)
            {
                backPageKey = selectedNode->getNodeKey();
            }
            else
            {
                if (_navigationModel->getSelectedNode())
                {
                    backPageKey = _navigationModel->getSelectedNode()->getNodeKey();
                }
            }
            routeData.insert("ElaBackPageKey", backPageKey);
            routeData.insert("ElaForwardPageKey", node->getNodeKey());
            ElaNavigationRouter::getInstance()->navigationRoute(this, "onNavigationRoute", routeData);
        }
        Q_EMIT q->navigationNodeClicked(ElaNavigationType::FooterNode, node->getNodeKey(), isRouteBack);

        if (node->getIsHasFooterPage())
        {
            if (_navigationModel->getSelectedNode() || _navigationModel->getSelectedExpandedNode())
            {
                QVariantMap mainPostData = QVariantMap();
                mainPostData.insert("SelectMarkChanged", true);
                mainPostData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedExpandedNode() ? _navigationModel->getSelectedExpandedNode() : _navigationModel->getSelectedNode()));
                mainPostData.insert("SelectedNode", QVariant::fromValue(nullptr));
                _navigationView->clearSelection();
                _navigationView->navigationNodeStateChange(mainPostData);
                _navigationModel->setSelectedExpandedNode(nullptr);
                _navigationModel->setSelectedNode(nullptr);
            }
            _footerView->clearSelection();
            _footerView->selectionModel()->select(index, QItemSelectionModel::Select);
            QVariantMap postData = QVariantMap();
            postData.insert("SelectMarkChanged", true);
            postData.insert("LastSelectedNode", QVariant::fromValue(_footerModel->getSelectedNode()));
            postData.insert("SelectedNode", QVariant::fromValue(node));
            _footerDelegate->navigationNodeStateChange(postData);
            _footerModel->setSelectedNode(node);
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
        if (_navigationModel->hasChildren(index))
        {
            _initNodeModelIndex(index);
        }
    }
}

void ElaNavigationBarPrivate::_resetNodeSelected()
{
    _navigationView->clearSelection();
    ElaNavigationNode* selectedNode = _navigationModel->getSelectedNode();
    if (!selectedNode || !selectedNode->getModelIndex().isValid())
    {
        return;
    }
    if (selectedNode->getParentNode()->getIsRootNode() || selectedNode->getIsVisible())
    {
        _navigationView->selectionModel()->select(selectedNode->getModelIndex(), QItemSelectionModel::Select);
        if (_navigationModel->getSelectedExpandedNode())
        {
            QVariantMap postData = QVariantMap();
            postData.insert("SelectMarkChanged", true);
            postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedExpandedNode()));
            postData.insert("SelectedNode", QVariant::fromValue(selectedNode));
            _navigationView->navigationNodeStateChange(postData);
        }
        _navigationModel->setSelectedExpandedNode(nullptr);
    }
    else
    {
        ElaNavigationNode* parentNode = selectedNode->getParentNode();
        while (parentNode && !parentNode->getParentNode()->getIsRootNode() && !parentNode->getIsVisible())
        {
            parentNode = parentNode->getParentNode();
        }
        if (!parentNode)
        {
            return;
        }
        // 单级节点展开/收起时Mark变化
        if (!_navigationModel->getSelectedExpandedNode())
        {
            QVariantMap postData = QVariantMap();
            postData.insert("SelectMarkChanged", true);
            postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedNode()));
            postData.insert("SelectedNode", QVariant::fromValue(parentNode));
            _navigationView->navigationNodeStateChange(postData);
        }
        else
        {
            // 多级节点
            if (_navigationModel->getSelectedExpandedNode() != parentNode)
            {
                // 同一起源节点展开/收起时的Mark变化
                if (_navigationModel->getSelectedExpandedNode()->getOriginalNode() == parentNode->getOriginalNode())
                {
                    QVariantMap postData = QVariantMap();
                    postData.insert("SelectMarkChanged", true);
                    postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedExpandedNode()));
                    postData.insert("SelectedNode", QVariant::fromValue(parentNode));
                    _navigationView->navigationNodeStateChange(postData);
                }
            }
        }
        _navigationModel->setSelectedExpandedNode(parentNode);
        _navigationView->selectionModel()->select(parentNode->getModelIndex(), QItemSelectionModel::Select);
    }
}

void ElaNavigationBarPrivate::_expandSelectedNodeParent()
{
    ElaNavigationNode* parentNode = _navigationModel->getSelectedNode()->getParentNode();
    while (parentNode && !parentNode->getIsRootNode())
    {
        QVariantMap data;
        data.insert("Expand", QVariant::fromValue(parentNode));
        _navigationView->navigationNodeStateChange(data);
        parentNode->setIsExpanded(true);
        _navigationView->expand(parentNode->getModelIndex());
        parentNode = parentNode->getParentNode();
    }
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
        QModelIndex index = node->getModelIndex();
        bool isExpanded = _navigationView->isExpanded(index);
        if (node->getIsHasChild() && isExpand != isExpanded)
        {
            QVariantMap data;
            if (isExpanded)
            {
                // 收起
                data.insert("Collapse", QVariant::fromValue(node));
                node->setIsExpanded(isExpand);
                _navigationView->navigationNodeStateChange(data);
                _navigationView->collapse(index);
            }
            else
            {
                // 展开
                data.insert("Expand", QVariant::fromValue(node));
                node->setIsExpanded(true);
                _navigationView->navigationNodeStateChange(data);
                _navigationView->expand(index);
            }
        }
    }
}

void ElaNavigationBarPrivate::_addStackedPage(QWidget* page, QString pageKey)
{
    Q_Q(ElaNavigationBar);
    page->setProperty("ElaPageKey", pageKey);
    Q_EMIT q->navigationNodeAdded(ElaNavigationType::PageNode, pageKey, page);
    ElaNavigationNode* node = _navigationModel->getNavigationNode(pageKey);
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
    _footerView->setFixedHeight(40 * _footerModel->getFooterNodeCount());
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
    QTimer::singleShot(200, this, [=]() {
        if (_currentDisplayMode == ElaNavigationType::Compact)
        {
            return;
        }
        QRect indexRect = _navigationView->visualRect(index);
        QRect viewportRect = _navigationView->viewport()->rect();
        if (viewportRect.contains(indexRect))
        {
            return;
        }
        auto vScrollBar = _navigationView->verticalScrollBar();
        int startValue = vScrollBar->value();
        int endValue = startValue + indexRect.top() - ((viewportRect.height() - indexRect.height()) / 2);
        QPropertyAnimation* scrollAnimation = new QPropertyAnimation(vScrollBar, "value");
        scrollAnimation->setEasingCurve(QEasingCurve::OutSine);
        scrollAnimation->setDuration(255);
        scrollAnimation->setStartValue(startValue);
        scrollAnimation->setEndValue(endValue);
        scrollAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    });
}

void ElaNavigationBarPrivate::_doComponentAnimation(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation)
{
    switch (displayMode)
    {
    case ElaNavigationType::Minimal:
    {
        _doNavigationBarWidthAnimation(displayMode, isAnimation);
        if (_currentDisplayMode == ElaNavigationType::Maximal)
        {
            _userCard->setVisible(false);
            if (_isShowUserCard)
            {
                _userButton->setVisible(true);
            }
            _handleNavigationExpandState(true);
        }
        _currentDisplayMode = displayMode;
        break;
    }
    case ElaNavigationType::Compact:
    {
        _doNavigationBarWidthAnimation(displayMode, isAnimation);
        _doNavigationViewWidthAnimation(isAnimation);
        if (_currentDisplayMode != ElaNavigationType::Minimal)
        {
            _handleUserButtonLayout(true);
            _doUserButtonAnimation(true, isAnimation);
            _handleNavigationExpandState(true);
        }
        _currentDisplayMode = displayMode;
        break;
    }
    case ElaNavigationType::Maximal:
    {
        _handleUserButtonLayout(false);
        _doNavigationBarWidthAnimation(displayMode, isAnimation);
        _doUserButtonAnimation(false, isAnimation);
        _currentDisplayMode = displayMode;
        _handleNavigationExpandState(false);
        break;
    }
    default:
    {
        break;
    }
    }
}

void ElaNavigationBarPrivate::_handleNavigationExpandState(bool isSave)
{
    if (isSave)
    {
        // 保存展开状态 收起根节点所有子树
        _lastExpandedNodesList = _navigationModel->getRootExpandedNodes();
        for (auto node: _lastExpandedNodesList)
        {
            onTreeViewClicked(node->getModelIndex(), false);
        }
    }
    else
    {
        // 修正动画覆盖
        _navigationView->resize(_pNavigationBarWidth - 5, _navigationView->height());
        for (auto node: _lastExpandedNodesList)
        {
            onTreeViewClicked(node->getModelIndex(), false);
        }
    }
}

void ElaNavigationBarPrivate::_handleUserButtonLayout(bool isCompact)
{
    while (_userButtonLayout->count())
    {
        _userButtonLayout->takeAt(0);
    }
    if (_isShowUserCard)
    {
        _userButtonLayout->addSpacing(isCompact ? 36 : 80);
    }
}

void ElaNavigationBarPrivate::_resetLayout()
{
    while (_userButtonLayout->count())
    {
        _userButtonLayout->takeAt(0);
    }
    _userButtonLayout->addWidget(_userButton);
}

void ElaNavigationBarPrivate::_doNavigationBarWidthAnimation(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation)
{
    Q_Q(ElaNavigationBar);
    QPropertyAnimation* navigationBarWidthAnimation = new QPropertyAnimation(q, "maximumWidth");
    navigationBarWidthAnimation->setEasingCurve(QEasingCurve::OutCubic);
    navigationBarWidthAnimation->setStartValue(q->width());
    navigationBarWidthAnimation->setDuration(isAnimation ? 255 : 0);
    switch (displayMode)
    {
    case ElaNavigationType::Minimal:
    {
        connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            q->setFixedWidth(value.toUInt());
        });
        navigationBarWidthAnimation->setEndValue(0);
        break;
    }
    case ElaNavigationType::Compact:
    {
        connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            q->setFixedWidth(value.toUInt());
        });
        navigationBarWidthAnimation->setEndValue(42);
        break;
    }
    case ElaNavigationType::Maximal:
    {
        connect(navigationBarWidthAnimation, &QPropertyAnimation::finished, this, [=]() {
            _resetLayout();
        });
        connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            q->setFixedWidth(value.toUInt());
        });
        navigationBarWidthAnimation->setEndValue(_pNavigationBarWidth);
        break;
    }
    default:
    {
        break;
    }
    }
    navigationBarWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaNavigationBarPrivate::_doNavigationViewWidthAnimation(bool isAnimation)
{
    QPropertyAnimation* navigationViewWidthAnimation = new QPropertyAnimation(this, "pNavigationViewWidth");
    connect(navigationViewWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _navigationView->setColumnWidth(0, value.toUInt());
    });
    navigationViewWidthAnimation->setEasingCurve(QEasingCurve::OutCubic);
    navigationViewWidthAnimation->setStartValue(_navigationView->columnWidth(0));
    navigationViewWidthAnimation->setEndValue(40);
    navigationViewWidthAnimation->setDuration(isAnimation ? 255 : 0);
    navigationViewWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaNavigationBarPrivate::_doUserButtonAnimation(bool isCompact, bool isAnimation)
{
    QPropertyAnimation* userButtonAnimation = new QPropertyAnimation(_userButton, "geometry");
    connect(userButtonAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _userButton->setFixedSize(value.toRect().size());
    });
    userButtonAnimation->setEasingCurve(isCompact ? QEasingCurve::OutCubic : QEasingCurve::InOutSine);
    QRect maximumRect = QRect(13, 18, 64, 64);
    QRect compactRect = QRect(3, 10, 36, 36);
    userButtonAnimation->setStartValue(isCompact ? maximumRect : compactRect);
    userButtonAnimation->setEndValue(isCompact ? compactRect : maximumRect);

    QPropertyAnimation* spacingAnimation = new QPropertyAnimation(this, "pUserButtonSpacing");
    connect(spacingAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        while (_userButtonLayout->count())
        {
            _userButtonLayout->takeAt(0);
        }
        if (_isShowUserCard)
        {
            _userButtonLayout->addSpacing(value.toInt());
        }
    });
    spacingAnimation->setEasingCurve(isCompact ? QEasingCurve::OutCubic : QEasingCurve::InOutSine);
    spacingAnimation->setStartValue(isCompact ? 80 : 36);
    spacingAnimation->setEndValue(isCompact ? 36 : 80);
    if (isCompact)
    {
        _userCard->setVisible(false);
        if (_isShowUserCard)
        {
            _userButton->setVisible(true);
        }
        userButtonAnimation->setDuration(isAnimation ? 255 : 0);
        spacingAnimation->setDuration(isAnimation ? 255 : 0);
    }
    else
    {
        connect(spacingAnimation, &QPropertyAnimation::finished, this, [=]() {
            _userButton->setFixedSize(36, 36);
            _userButton->setGeometry(QRect(3, 10, 36, 36));
            _userButton->setVisible(false);
            _resetLayout();
            if (_isShowUserCard)
            {
                _userCard->setVisible(true);
            }
        });
        userButtonAnimation->setDuration(isAnimation ? 135 : 0);
        spacingAnimation->setDuration(isAnimation ? 135 : 0);
    }
    userButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    spacingAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
