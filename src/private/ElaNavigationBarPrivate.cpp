#include "ElaNavigationBarPrivate.h"

#include <QLayout>
#include <QPropertyAnimation>

#include "ElaBaseListView.h"
#include "ElaCompactDelegate.h"
#include "ElaCompactModel.h"
#include "ElaFooterDelegate.h"
#include "ElaFooterModel.h"
#include "ElaMenu.h"
#include "ElaNavigationBar.h"
#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationRouter.h"
#include "ElaNavigationView.h"
#include "ElaScrollPagePrivate.h"
#include "ElaSuggestBox.h"
#include "ElaSuggestBoxPrivate.h"
#include "ElaTheme.h"
ElaNavigationBarPrivate::ElaNavigationBarPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaNavigationBarPrivate::~ElaNavigationBarPrivate()
{
}

void ElaNavigationBarPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    _windowLinearGradient->setColorAt(0, ElaThemeColor(themeMode, NavigationBaseStart));
    _windowLinearGradient->setColorAt(1, ElaThemeColor(themeMode, NavigationBaseEnd));
}

void ElaNavigationBarPrivate::onNavigationRouteBack(QVariantMap routeData)
{
    Q_Q(ElaNavigationBar);
    QString pageKey = routeData.value("ElaPageKey").toString();
    q->navigation(pageKey, false);
}

void ElaNavigationBarPrivate::onTreeViewClicked(const QModelIndex& index, bool isLogRoute)
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
            if (node->getIsHasChild())
            {
                QVariantMap data;
                if (_navigationView->isExpanded(index))
                {
                    // 收起
                    data.insert("Collapse", QVariant::fromValue(node));
                    node->setIsExpanded(false);
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
                    QStringList pageKeyList;
                    if (selectedNode)
                    {
                        pageKeyList.append(selectedNode->getNodeKey());
                    }
                    else
                    {
                        if (_footerModel->getSelectedNode())
                        {
                            pageKeyList.append(_footerModel->getSelectedNode()->getNodeKey());
                        }
                    }
                    routeData.insert("ElaPageKey", pageKeyList);
                    ElaNavigationRouter::getInstance()->navigationRoute(this, "onNavigationRouteBack", routeData);
                }
                Q_EMIT q->navigationNodeClicked(ElaNavigationType::PageNode, node->getNodeKey());
                QVariantMap compactPostData = QVariantMap();
                compactPostData.insert("SelectMarkChanged", true);
                if (_footerModel->getSelectedNode())
                {
                    _footerView->clearSelection();
                    QVariantMap footerPostData = QVariantMap();
                    footerPostData.insert("SelectMarkChanged", true);
                    footerPostData.insert("LastSelectedNode", QVariant::fromValue(_footerModel->getSelectedNode()));
                    footerPostData.insert("SelectedNode", QVariant::fromValue(nullptr));
                    compactPostData.insert("LastSelectedNode", QVariant::fromValue(_footerModel->getSelectedNode()));
                    _footerModel->setSelectedNode(nullptr);
                    _footerDelegate->navigationNodeStateChange(footerPostData);
                }
                QVariantMap postData = QVariantMap();
                postData.insert("SelectMarkChanged", true);
                if (_navigationModel->getSelectedNode())
                {
                    compactPostData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedNode()->getOriginalNode()));
                }
                if (_navigationModel->getSelectedExpandedNode())
                {
                    postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedExpandedNode()));
                }
                else
                {
                    postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedNode()));
                }
                postData.insert("SelectedNode", QVariant::fromValue(node));
                ElaNavigationNode* originalNode = node->getOriginalNode();
                compactPostData.insert("SelectedNode", QVariant::fromValue(originalNode));
                //避免同一起源节点下的无效Mark动画
                if (originalNode != _compactModel->getSelectedNode())
                {
                    _compactModel->setSelectedNode(originalNode);
                    _compactDelegate->navigationNodeStateChange(compactPostData);
                }
                _navigationModel->setSelectedNode(node);
                _navigationView->navigationNodeStateChange(postData);
                if (!node->getIsVisible())
                {
                    _expandSelectedNodeParent();
                }
            }
        }
        _resetNodeSelected();
    }
}

void ElaNavigationBarPrivate::onFooterViewClicked(const QModelIndex& index, bool isLogRoute)
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
            QStringList pageKeyList;
            if (selectedNode)
            {
                pageKeyList.append(selectedNode->getNodeKey());
            }
            else
            {
                if (_navigationModel->getSelectedNode())
                {
                    pageKeyList.append(_navigationModel->getSelectedNode()->getNodeKey());
                }
            }
            routeData.insert("ElaPageKey", pageKeyList);
            ElaNavigationRouter::getInstance()->navigationRoute(this, "onNavigationRouteBack", routeData);
        }
        Q_EMIT q->navigationNodeClicked(ElaNavigationType::FooterNode, node->getNodeKey());

        if (node->getIsHasFooterPage())
        {
            QVariantMap compactPostData = QVariantMap();
            compactPostData.insert("SelectMarkChanged", true);
            if (_navigationModel->getSelectedNode() || _navigationModel->getSelectedExpandedNode())
            {
                QVariantMap mainPostData = QVariantMap();
                mainPostData.insert("SelectMarkChanged", true);
                mainPostData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedExpandedNode() ? _navigationModel->getSelectedExpandedNode() : _navigationModel->getSelectedNode()));
                mainPostData.insert("SelectedNode", QVariant::fromValue(nullptr));
                compactPostData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedNode()->getOriginalNode()));
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
            compactPostData.insert("SelectedNode", QVariant::fromValue(node));
            _compactModel->setSelectedNode(node);
            _compactDelegate->navigationNodeStateChange(compactPostData);
            _footerDelegate->navigationNodeStateChange(postData);
            _footerModel->setSelectedNode(node);
        }
    }
}

void ElaNavigationBarPrivate::onCompactViewClicked(const QModelIndex& index)
{
    ElaNavigationNode* node = index.data(Qt::UserRole).value<ElaNavigationNode*>();
    if (node->getIsFooterNode())
    {
        this->onFooterViewClicked(node->getModelIndex());
    }
    else
    {
        if (node->getIsExpanderNode())
        {
            if (node->getIsHasPageChild())
            {
                //展开菜单
                ElaMenu* menu = _compactMenuMap.value(node);
                if (menu)
                {
                    menu->popup(QCursor::pos());
                }
            }
        }
        else
        {
            this->onTreeViewClicked(node->getModelIndex());
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

void ElaNavigationBarPrivate::_addStackedPage(QWidget* page, QString pageKey)
{
    Q_Q(ElaNavigationBar);
    page->setProperty("ElaPageKey", pageKey);
    Q_EMIT q->navigationNodeAdded(ElaNavigationType::PageNode, pageKey, page);
    ElaNavigationNode* node = _navigationModel->getNavigationNode(pageKey);
    QVariantMap suggestData;
    suggestData.insert("ElaNodeType", "Stacked");
    suggestData.insert("ElaPageKey", pageKey);
    _navigationSuggestBox->addSuggestion(node->getAwesome(), node->getNodeTitle(), suggestData);
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
    _navigationSuggestBox->addSuggestion(node->getAwesome(), node->getNodeTitle(), suggestData);
}

void ElaNavigationBarPrivate::_raiseNavigationBar()
{
    Q_Q(ElaNavigationBar);
    q->raise();
}

void ElaNavigationBarPrivate::_switchContentLayout(bool direction)
{
    if (direction)
    {
        while (_mainLayout->count() > 0)
        {
            _mainLayout->takeAt(0);
        }
        _mainLayout->addWidget(_compactWidget);
        _maximalWidget->setVisible(false);
        _compactWidget->setVisible(true);
    }
    else
    {
        while (_mainLayout->count() > 0)
        {
            _mainLayout->takeAt(0);
        }
        _mainLayout->addWidget(_maximalWidget);
        _maximalWidget->setVisible(true);
        _compactWidget->setVisible(false);
    }
}

void ElaNavigationBarPrivate::_startContentWidgetAnimation(QPoint startPoint, QPoint endPoint, bool isAnimation)
{
    Q_Q(ElaNavigationBar);
    QPropertyAnimation* maximalAnimation = new QPropertyAnimation(_compactWidget, "pos");
    connect(maximalAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _compactWidget->resize(_compactWidget->width(), q->height());
        _maximalWidget->resize(_maximalWidget->width(), q->height());
        _maximalWidget->move(value.toPoint().x() - 300, value.toPoint().y());
    });
    maximalAnimation->setEasingCurve(QEasingCurve::InOutSine);
    maximalAnimation->setDuration(isAnimation ? 300 : 0);
    maximalAnimation->setStartValue(startPoint);
    maximalAnimation->setEndValue(endPoint);
    maximalAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaNavigationBarPrivate::_resetNavigationLayout(ElaNavigationType::NavigationDisplayMode displayMode)
{
    Q_Q(ElaNavigationBar);
    switch (displayMode)
    {
    case ElaNavigationType::Minimal:
    {
        while (_mainLayout->count() > 0)
        {
            _mainLayout->takeAt(0);
        }
        _compactWidget->resize(40, q->height());
        _compactWidget->setVisible(true);
        _maximalWidget->move(_compactWidget->pos().x() - 300, _maximalWidget->pos().y());
        break;
    }
    case ElaNavigationType::Compact:
    {
        break;
    }
    case ElaNavigationType::Maximal:
    {
        break;
    }
    default:
    {
        return;
    }
    }
}
