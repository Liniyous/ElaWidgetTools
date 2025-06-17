#include "ElaNavigationBarPrivate.h"

#include <QLayout>
#include <QPropertyAnimation>

#include "ElaApplication.h"
#include "ElaBaseListView.h"
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
#include "ElaSuggestBox.h"
#include "ElaSuggestBoxPrivate.h"
#include "ElaToolButton.h"

ElaNavigationBarPrivate::ElaNavigationBarPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaNavigationBarPrivate::~ElaNavigationBarPrivate()
{
}

void ElaNavigationBarPrivate::onNavigationButtonClicked()
{
    Q_Q(ElaNavigationBar);
    if (_currentDisplayMode == ElaNavigationType::Compact)
    {
        q->setDisplayMode(ElaNavigationType::Maximal);
    }
    else
    {
        q->setDisplayMode(ElaNavigationType::Compact);
    }
}

void ElaNavigationBarPrivate::onNavigationOpenNewWindow(QString nodeKey)
{
    Q_Q(ElaNavigationBar);
    const QMetaObject* meta = _pageMetaMap.value(nodeKey);
    if (!meta)
    {
        return;
    }
    QWidget* widget = static_cast<QWidget*>(meta->newInstance());
    if (widget)
    {
        _pageNewWindowCountMap[nodeKey] += 1;
        ElaCustomWidget* floatWidget = new ElaCustomWidget(q);
        connect(floatWidget, &ElaCustomWidget::customWidgetClosed, this, [=]() {
            _pageNewWindowCountMap[nodeKey] -= 1;
        });
        floatWidget->setWindowIcon(widget->windowIcon());
        floatWidget->setWindowTitle(widget->windowTitle());
        floatWidget->setCentralWidget(widget);
        floatWidget->show();
        Q_EMIT q->pageOpenInNewWindow(nodeKey);
    }
}

void ElaNavigationBarPrivate::onNavigationRouteBack(QVariantMap routeData)
{
    Q_Q(ElaNavigationBar);
    QString pageKey = routeData.value("ElaPageKey").toString();
    q->navigation(pageKey, false, true);
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
            _expandOrCollpaseExpanderNode(node, !_navigationView->isExpanded(index));
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
                    QString pageKey;
                    if (selectedNode)
                    {
                        pageKey.append(selectedNode->getNodeKey());
                    }
                    else
                    {
                        if (_footerModel->getSelectedNode())
                        {
                            pageKey.append(_footerModel->getSelectedNode()->getNodeKey());
                        }
                    }
                    routeData.insert("ElaPageKey", pageKey);
                    ElaNavigationRouter::getInstance()->navigationRoute(this, "onNavigationRouteBack", routeData);
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
            QString pageKey;
            if (selectedNode)
            {
                pageKey.append(selectedNode->getNodeKey());
            }
            else
            {
                if (_navigationModel->getSelectedNode())
                {
                    pageKey.append(_navigationModel->getSelectedNode()->getNodeKey());
                }
            }
            routeData.insert("ElaPageKey", pageKey);
            ElaNavigationRouter::getInstance()->navigationRoute(this, "onNavigationRouteBack", routeData);
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

void ElaNavigationBarPrivate::_expandOrCollpaseExpanderNode(ElaNavigationNode* node, bool isExpand)
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
    suggestData.insert("ElaNodeType", "Stacked");
    suggestData.insert("ElaPageKey", pageKey);
    QString suggestKey = _navigationSuggestBox->addSuggestion(node->getAwesome(), node->getNodeTitle(), suggestData);
    _suggestKeyMap.insert(pageKey, suggestKey);
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
    suggestData.insert("ElaNodeType", "Footer");
    suggestData.insert("ElaPageKey", footKey);
    QString suggestKey = _navigationSuggestBox->addSuggestion(node->getAwesome(), node->getNodeTitle(), suggestData);
    _suggestKeyMap.insert(footKey, suggestKey);
}

void ElaNavigationBarPrivate::_raiseNavigationBar()
{
    Q_Q(ElaNavigationBar);
    q->raise();
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
            _searchButton->setVisible(true);
            _userCard->setVisible(false);
            _navigationSuggestBox->setVisible(false);
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
            _handleMaximalToCompactLayout();
            _doNavigationButtonAnimation(true, isAnimation);
            _doSearchButtonAnimation(true, isAnimation);
            _doUserButtonAnimation(true, isAnimation);
            _navigationSuggestBox->setVisible(false);
            _handleNavigationExpandState(true);
        }
        _currentDisplayMode = displayMode;
        break;
    }
    case ElaNavigationType::Maximal:
    {
        _resetLayout();
        _handleCompactToMaximalLayout();
        _doNavigationBarWidthAnimation(displayMode, isAnimation);
        _doUserButtonAnimation(false, isAnimation);
        _doNavigationButtonAnimation(false, isAnimation);
        _doSearchButtonAnimation(false, isAnimation);
        _navigationSuggestBox->setVisible(true);
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

void ElaNavigationBarPrivate::_handleMaximalToCompactLayout()
{
    // 动画过程布局
    while (_navigationButtonLayout->count())
    {
        _navigationButtonLayout->takeAt(0);
    }
    if (_isShowUserCard)
    {
        _navigationButtonLayout->addSpacing(76);
    }
    else
    {
        _navigationButtonLayout->addSpacing(40);
    }

    _navigationSuggestLayout->addStretch();

    while (_userButtonLayout->count())
    {
        _userButtonLayout->takeAt(0);
    }
    _userButtonLayout->addSpacing(36);
}

void ElaNavigationBarPrivate::_handleCompactToMaximalLayout()
{
    // 动画过程布局
    while (_navigationButtonLayout->count())
    {
        _navigationButtonLayout->takeAt(0);
    }
    _navigationButtonLayout->addSpacing(38);
    _navigationSuggestLayout->insertSpacing(0, 46);

    while (_userButtonLayout->count())
    {
        _userButtonLayout->takeAt(0);
    }
    if (_isShowUserCard)
    {
        _userButtonLayout->addSpacing(74);
    }
}

void ElaNavigationBarPrivate::_resetLayout()
{
    // 恢复初始布局
    while (_navigationButtonLayout->count())
    {
        _navigationButtonLayout->takeAt(0);
    }
    _navigationButtonLayout->addWidget(_navigationButton);
    _navigationButtonLayout->addWidget(_searchButton);

    while (_navigationSuggestLayout->count())
    {
        _navigationSuggestLayout->takeAt(0);
    }
    _navigationSuggestLayout->addLayout(_navigationButtonLayout);
    _navigationSuggestLayout->addWidget(_navigationSuggestBox);

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
    navigationBarWidthAnimation->setDuration(isAnimation ? 285 : 0);
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
        navigationBarWidthAnimation->setEndValue(47);
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
    navigationViewWidthAnimation->setDuration(isAnimation ? 285 : 0);
    navigationViewWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaNavigationBarPrivate::_doNavigationButtonAnimation(bool isCompact, bool isAnimation)
{
    if (isCompact)
    {
        // 导航按钮
        QPropertyAnimation* navigationButtonAnimation = new QPropertyAnimation(_navigationButton, "pos");
        connect(navigationButtonAnimation, &QPropertyAnimation::finished, this, [=]() {
            _resetLayout();
        });
        QPoint navigationButtonPos = _navigationButton->pos();
        navigationButtonAnimation->setStartValue(navigationButtonPos);
        if (_isShowUserCard)
        {
            navigationButtonAnimation->setEndValue(QPoint(0, 56));
        }
        else
        {
            navigationButtonAnimation->setEndValue(navigationButtonPos);
        }
        navigationButtonAnimation->setEasingCurve(QEasingCurve::OutCubic);
        navigationButtonAnimation->setDuration(isAnimation ? 285 : 0);
        navigationButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        QPropertyAnimation* navigationButtonAnimation = new QPropertyAnimation(_navigationButton, "pos");
        QPoint navigationButtonPos = _navigationButton->pos();
        navigationButtonAnimation->setStartValue(navigationButtonPos);
        if (_isShowUserCard)
        {
            navigationButtonAnimation->setEndValue(QPoint(0, 94));
        }
        else
        {
            navigationButtonAnimation->setEndValue(navigationButtonPos);
        }
        navigationButtonAnimation->setEasingCurve(QEasingCurve::InOutSine);
        navigationButtonAnimation->setDuration(isAnimation ? 130 : 0);
        navigationButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void ElaNavigationBarPrivate::_doSearchButtonAnimation(bool isCompact, bool isAnimation)
{
    if (isCompact)
    {
        QPoint navigationButtonPos = _navigationButton->pos();
        // 搜索按钮
        QPropertyAnimation* searchButtonAnimation = new QPropertyAnimation(_searchButton, "pos");
        if (_isShowUserCard)
        {
            searchButtonAnimation->setStartValue(QPoint(200, navigationButtonPos.y()));
            searchButtonAnimation->setEndValue(QPoint(0, navigationButtonPos.y()));
        }
        else
        {
            searchButtonAnimation->setStartValue(QPoint(200, navigationButtonPos.y() + 38));
            searchButtonAnimation->setEndValue(QPoint(0, navigationButtonPos.y() + 38));
        }
        searchButtonAnimation->setEasingCurve(QEasingCurve::OutCubic);
        searchButtonAnimation->setDuration(isAnimation ? 285 : 0);
        searchButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        _searchButton->setVisible(true);
    }
    else
    {
        _searchButton->setVisible(false);
    }
}

void ElaNavigationBarPrivate::_doUserButtonAnimation(bool isCompact, bool isAnimation)
{
    if (isCompact)
    {
        _userCard->setVisible(false);
        if (_isShowUserCard)
        {
            _userButton->setVisible(true);
        }
        QPropertyAnimation* userButtonAnimation = new QPropertyAnimation(_userButton, "geometry");
        connect(userButtonAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            _userButton->setFixedSize(value.toRect().size());
        });
        userButtonAnimation->setEasingCurve(QEasingCurve::OutCubic);
        userButtonAnimation->setStartValue(QRect(13, 18, 64, 64));
        userButtonAnimation->setEndValue(QRect(3, 10, 36, 36));
        userButtonAnimation->setDuration(isAnimation ? 285 : 0);
        userButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        QPropertyAnimation* userButtonAnimation = new QPropertyAnimation(_userButton, "geometry");
        connect(userButtonAnimation, &QPropertyAnimation::finished, this, [=]() {
            if (_isShowUserCard)
            {
                _userCard->setVisible(true);
            }
            _userButton->setFixedSize(36, 36);
            _userButton->setGeometry(QRect(3, 10, 36, 36));
            _userButton->setVisible(false);
        });
        connect(userButtonAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            _userButton->setFixedSize(value.toRect().size());
        });
        userButtonAnimation->setEasingCurve(QEasingCurve::InOutSine);
        userButtonAnimation->setStartValue(QRect(3, 10, 36, 36));
        userButtonAnimation->setEndValue(QRect(13, 18, 64, 64));
        userButtonAnimation->setDuration(isAnimation ? 130 : 0);
        userButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
