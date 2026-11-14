#include "ElaNavigationBar.h"

#include "ElaBaseListView.h"
#include "ElaColorAnimation.h"
#include "ElaFooterDelegate.h"
#include "ElaFooterModel.h"
#include "ElaIconButton.h"
#include "ElaInteractiveCard.h"
#include "ElaMenu.h"
#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationView.h"
#include "ElaSuggestBox.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"
#include "private/ElaNavigationBarPrivate.h"

#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QScroller>
#include <QVBoxLayout>

ELA_PROPERTY_Q_CPP(ElaNavigationBar, bool, IsTransparent)
ELA_PROPERTY_Q_CPP(ElaNavigationBar, bool, IsAllowPageOpenInNewWindow)
ElaNavigationBar::ElaNavigationBar(QWidget* parent)
    : QWidget{parent}
{
    d_ptr.reset(new ElaNavigationBarPrivate());
    Q_D(ElaNavigationBar);
    d->q_ptr = this;
    d->_pIsAllowPageOpenInNewWindow = true;
    d->_pNavigationBarWidth = 300;
    setFixedWidth(300);
    d->_pIsTransparent = true;

    //用户卡片
    d->_userCard = new ElaInteractiveCard(this);
    d->_userCard->setMinimumWidth(0);
    d->_userCard->setCardPixmap(QPixmap(":/include/Image/Cirno.jpg"));
    d->_userCard->setTitle("Ela 工具");
    d->_userCard->setSubTitle("Liniyous@gmail.com");
    connect(d->_userCard, &ElaInteractiveCard::clicked, this, &ElaNavigationBar::userInfoCardClicked);
    d->_userButton = new ElaIconButton(QPixmap(":/include/Image/Cirno.jpg"), this);
    d->_userButton->setFixedSize(36, 36);
    d->_userButton->setVisible(false);
    d->_userButton->setBorderRadius(8);
    connect(d->_userButton, &ElaIconButton::clicked, this, &ElaNavigationBar::userInfoCardClicked);
    d->_userCardLayout = new QVBoxLayout();
    d->_userCardLayout->setContentsMargins(3, 10, 0, 5);
    d->_userCardLayout->addWidget(d->_userButton, 0, Qt::AlignLeft);
    d->_userCardLayout->addWidget(d->_userCard);

    // 导航模型
    d->_navigationModel = new ElaNavigationModel(this);
    d->_navigationView = new ElaNavigationView(this);
    d->_navigationView->setNavigationBarPrivate(d);
    d->_navigationView->setModel(d->_navigationModel);
    connect(d->_navigationView, &ElaNavigationView::navigationClicked, this, [=](const QModelIndex& index) {
        d->onTreeViewClicked(index);
    });
    connect(d->_navigationView, &ElaNavigationView::navigationOpenNewWindow, d, &ElaNavigationBarPrivate::onNavigationOpenNewWindow);

    // 页脚
    d->_footerView = new ElaBaseListView(this);
    d->_footerView->setFixedHeight(0);
    d->_footerModel = new ElaFooterModel(this);
    d->_footerView->setModel(d->_footerModel);
    d->_footerDelegate = new ElaFooterDelegate(d->_footerView, this);
    d->_footerView->setItemDelegate(d->_footerDelegate);
    connect(d->_footerView, &ElaBaseListView::mousePress, this, [=](const QModelIndex& index) {
        d->_footerDelegate->setPressIndex(index);
        d->_footerView->viewport()->update();
    });
    connect(d->_footerView, &ElaBaseListView::mouseDoubleClick, this, [=](const QModelIndex& index) {
        d->_footerDelegate->setPressIndex(index);
        d->_footerView->viewport()->update();
    });
    connect(d->_footerView, &ElaBaseListView::mouseRelease, this, [=](const QModelIndex& index) {
        d->_footerDelegate->setPressIndex(QModelIndex());
        d->_footerView->viewport()->update();
    });
    connect(d->_footerView, &ElaBaseListView::clicked, this, [=](const QModelIndex& index) {
        d->onFooterViewClicked(index);
    });

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setSpacing(0);
    // 内容先切换到目标宽度 外层宽度动画负责裁剪与页面布局过渡
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mainLayout->addLayout(d->_userCardLayout);
    mainLayout->addWidget(d->_navigationView);
    mainLayout->addWidget(d->_footerView);
    d->_updateDisplayLayout();

    // 主题设置
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaNavigationBar::~ElaNavigationBar()
{
}

void ElaNavigationBar::setNavigationBarWidth(int navigationBarWidth)
{
    Q_D(ElaNavigationBar);
    if (navigationBarWidth < 180)
    {
        navigationBarWidth = 180;
    }
    d->_pNavigationBarWidth = navigationBarWidth;
    d->_updateDisplayLayout();
    if (d->_currentDisplayMode == ElaNavigationType::Maximal)
    {
        d->_doNavigationBarWidthAnimation(d->_currentDisplayMode, false);
    }
    Q_EMIT pNavigationBarWidthChanged();
}

int ElaNavigationBar::getNavigationBarWidth() const
{
    Q_D(const ElaNavigationBar);
    return d->_pNavigationBarWidth;
}

void ElaNavigationBar::setUserInfoCardVisible(bool isVisible)
{
    Q_D(ElaNavigationBar);
    d->_isShowUserCard = isVisible;
    d->_updateDisplayLayout();
}

void ElaNavigationBar::setUserInfoCardPixmap(const QPixmap& pix)
{
    Q_D(ElaNavigationBar);
    d->_userCard->setCardPixmap(pix);
    d->_userButton->setPixmap(pix);
}

void ElaNavigationBar::setUserInfoCardTitle(const QString& title)
{
    Q_D(ElaNavigationBar);
    d->_userCard->setTitle(title);
}

void ElaNavigationBar::setUserInfoCardSubTitle(const QString& subTitle)
{
    Q_D(ElaNavigationBar);
    d->_userCard->setSubTitle(subTitle);
}

ElaNavigationType::NodeResult ElaNavigationBar::addExpanderNode(const QString& expanderTitle, QString& expanderKey, ElaIconType::IconName awesome)
{
    Q_D(ElaNavigationBar);
    ElaNavigationType::NodeResult result = d_ptr->_navigationModel->addExpanderNode(expanderTitle, expanderKey, awesome);
    if (result == ElaNavigationType::Success)
    {
        d->_resetNodeSelected();
    }
    return result;
}

ElaNavigationType::NodeResult ElaNavigationBar::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, ElaIconType::IconName awesome)
{
    Q_D(ElaNavigationBar);
    ElaNavigationType::NodeResult result = d->_navigationModel->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awesome);
    if (result == ElaNavigationType::Success)
    {
        d->_resetNodeSelected();
    }
    return result;
}

ElaNavigationType::NodeResult ElaNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, ElaIconType::IconName awesome)
{
    Q_D(ElaNavigationBar);
    if (!page)
    {
        return ElaNavigationType::PageInvalid;
    }
    QString pageKey;
    ElaNavigationType::NodeResult result = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, awesome);
    if (result == ElaNavigationType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_pageNewWindowCountMap.insert(pageKey, 0);
        d->_addStackedPage(page, pageKey);
        d->_resetNodeSelected();
    }
    return result;
}

ElaNavigationType::NodeResult ElaNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, int keyPoints, ElaIconType::IconName awesome)
{
    Q_D(ElaNavigationBar);
    if (!page)
    {
        return ElaNavigationType::PageInvalid;
    }
    QString pageKey;
    ElaNavigationType::NodeResult result = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, keyPoints, awesome);
    if (result == ElaNavigationType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_pageNewWindowCountMap.insert(pageKey, 0);
        d->_addStackedPage(page, pageKey);
        d->_resetNodeSelected();
    }
    return result;
}

ElaNavigationType::NodeResult ElaNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, ElaIconType::IconName awesome)
{
    Q_D(ElaNavigationBar);
    if (!page)
    {
        return ElaNavigationType::PageInvalid;
    }
    if (targetExpanderKey.isEmpty())
    {
        return ElaNavigationType::TargetNodeInvalid;
    }
    QString pageKey;
    ElaNavigationType::NodeResult result = d->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, awesome);
    if (result == ElaNavigationType::NodeResult::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_pageNewWindowCountMap.insert(pageKey, 0);
        d_ptr->_addStackedPage(page, pageKey);
        d->_resetNodeSelected();
    }
    return result;
}

ElaNavigationType::NodeResult ElaNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints, ElaIconType::IconName awesome)
{
    Q_D(ElaNavigationBar);
    if (!page)
    {
        return ElaNavigationType::PageInvalid;
    }
    if (targetExpanderKey.isEmpty())
    {
        return ElaNavigationType::TargetNodeInvalid;
    }
    QString pageKey;
    ElaNavigationType::NodeResult result = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, keyPoints, awesome);
    if (result == ElaNavigationType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_pageNewWindowCountMap.insert(pageKey, 0);
        d_ptr->_addStackedPage(page, pageKey);
        d->_resetNodeSelected();
    }
    return result;
}

ElaNavigationType::NodeResult ElaNavigationBar::addFooterNode(const QString& footerTitle, QString& footerKey, int keyPoints, ElaIconType::IconName awesome)
{
    return addFooterNode(footerTitle, nullptr, footerKey, keyPoints, awesome);
}

ElaNavigationType::NodeResult ElaNavigationBar::addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, int keyPoints, ElaIconType::IconName awesome)
{
    ElaNavigationType::NodeResult result = d_ptr->_footerModel->addFooterNode(footerTitle, footerKey, page != nullptr, keyPoints, awesome);
    if (result == ElaNavigationType::Success)
    {
        d_ptr->_addFooterPage(page, footerKey);
    }
    return result;
}

ElaNavigationType::NodeResult ElaNavigationBar::addCategoryNode(const QString& categoryTitle, QString& categoryKey)
{
    Q_D(ElaNavigationBar);
    ElaNavigationType::NodeResult result = d_ptr->_navigationModel->addCategoryNode(categoryTitle, categoryKey);
    if (result == ElaNavigationType::Success)
    {
        d->_updateCategoryVisibility();
        d->_resetNodeSelected();
        Q_EMIT navigationNodeAdded(ElaNavigationType::CategoryNode, categoryKey, nullptr);
    }
    return result;
}

ElaNavigationType::NodeResult ElaNavigationBar::addCategoryNode(const QString& categoryTitle, QString& categoryKey, const QString& targetExpanderKey)
{
    Q_D(ElaNavigationBar);
    ElaNavigationType::NodeResult result = d_ptr->_navigationModel->addCategoryNode(categoryTitle, categoryKey, targetExpanderKey);
    if (result == ElaNavigationType::Success)
    {
        d->_updateCategoryVisibility();
        d->_resetNodeSelected();
    }
    return result;
}

bool ElaNavigationBar::getNodeIsExpanded(const QString& expanderKey) const
{
    Q_D(const ElaNavigationBar);
    ElaNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode())
    {
        return false;
    }
    return d->_navigationView->isExpanded(node->getModelIndex());
}

void ElaNavigationBar::expandNode(const QString& expanderKey)
{
    Q_D(ElaNavigationBar);
    ElaNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode())
    {
        return;
    }
    d->_expandOrCollapseExpanderNode(node, true);
    d->_resetNodeSelected();
}

void ElaNavigationBar::collapseNode(const QString& expanderKey)
{
    Q_D(ElaNavigationBar);
    ElaNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode())
    {
        return;
    }
    d->_expandOrCollapseExpanderNode(node, false);
    d->_resetNodeSelected();
}

void ElaNavigationBar::removeNode(const QString& nodeKey)
{
    Q_D(ElaNavigationBar);
    ElaNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(nodeKey);
    }
    if (!node)
    {
        return;
    }
    ElaNavigationNode* selectedNode = d->_getSelectedNode();
    if (selectedNode && (selectedNode == node || node->getIsChildNode(selectedNode)))
    {
        d->_smoothScrollNavigationView(QModelIndex());
        d->_setSelectedNode(nullptr);
    }
    const auto nodeType = node->getIsFooterNode() ? ElaNavigationType::FooterNode : node->getIsCategoryNode() ? ElaNavigationType::CategoryNode
                                                                                                              : ElaNavigationType::PageNode;
    QStringList removedKeys;
    if (node->getIsFooterNode())
    {
        d->_footerModel->removeNavigationNode(nodeKey);
        d->_footerView->setFixedHeight(40 * d->_footerModel->getFooterNodeCount());
        removedKeys.append(nodeKey);
    }
    else
    {
        removedKeys = d->_navigationModel->removeNavigationNode(nodeKey);
        d->_initNodeModelIndex(QModelIndex());
    }
    for (auto it = d->_compactMenuMap.begin(); it != d->_compactMenuMap.end();)
    {
        for (auto action: it.value()->actions())
        {
            if (removedKeys.contains(action->data().toString()))
            {
                it.value()->removeAction(action);
                action->deleteLater();
            }
        }
        if (it.key() == node || it.value()->isEmpty())
        {
            it.value()->deleteLater();
            it = d->_compactMenuMap.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (int i = d->_suggestDataList.count() - 1; i >= 0; i--)
    {
        if (removedKeys.contains(d->_suggestDataList[i].getSuggestData().value("ElaPageKey").toString()))
        {
            d->_suggestDataList.removeAt(i);
        }
    }
    d->_resetNodeSelected();
    for (const auto& removedKey: removedKeys)
    {
        d->_pageMetaMap.remove(removedKey);
        d->_pageNewWindowCountMap.remove(removedKey);
        Q_EMIT navigationNodeRemoved(nodeType, removedKey);
    }
}

void ElaNavigationBar::setNodeKeyPoints(const QString& nodeKey, int keyPoints)
{
    Q_D(ElaNavigationBar);
    ElaNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(nodeKey);
    }
    else if (node->getIsExpanderNode() || keyPoints < 0)
    {
        return;
    }
    if (!node)
    {
        return;
    }
    node->setKeyPoints(keyPoints);
    if (node->getIsFooterNode())
    {
        d->_footerView->viewport()->update();
    }
    else
    {
        d->_navigationView->viewport()->update();
    }
}

int ElaNavigationBar::getNodeKeyPoints(const QString& nodeKey) const
{
    Q_D(const ElaNavigationBar);
    ElaNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(nodeKey);
    }
    else if (node->getIsExpanderNode())
    {
        return -1;
    }
    if (!node)
    {
        return -1;
    }
    return node->getKeyPoints();
}

void ElaNavigationBar::setNodeTitle(const QString& nodeKey, const QString& nodeTitle)
{
    Q_D(ElaNavigationBar);
    ElaNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(nodeKey);
    }
    if (!node)
    {
        return;
    }
    node->setNodeTitle(nodeTitle);
    update();
}

QString ElaNavigationBar::getNodeTitle(const QString& nodeKey) const
{
    Q_D(const ElaNavigationBar);
    ElaNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(nodeKey);
    }
    if (!node)
    {
        return {};
    }
    return node->getNodeTitle();
}

void ElaNavigationBar::navigation(const QString& pageKey, bool isLogClicked, bool isRouteBack)
{
    Q_D(ElaNavigationBar);
    ElaNavigationNode* node = d->_navigationModel->getNavigationNode(pageKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(pageKey);
    }
    if (node)
    {
        if (node->getIsFooterNode())
        {
            d->onFooterViewClicked(node->getModelIndex(), isLogClicked, isRouteBack);
        }
        else
        {
            if (!node->getIsExpanderNode())
            {
                d->onTreeViewClicked(node->getModelIndex(), isLogClicked, isRouteBack, true);
            }
        }
    }
}

void ElaNavigationBar::setDisplayMode(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation)
{
    Q_D(ElaNavigationBar);
    if (d->_currentDisplayMode == displayMode || displayMode == ElaNavigationType::Auto)
    {
        return;
    }
    d->_doComponentAnimation(displayMode, isAnimation);
    d->_raiseNavigationBar();
}

ElaNavigationType::NavigationDisplayMode ElaNavigationBar::getDisplayMode() const
{
    Q_D(const ElaNavigationBar);
    return d->_currentDisplayMode;
}

int ElaNavigationBar::getPageOpenInNewWindowCount(const QString& nodeKey) const
{
    Q_D(const ElaNavigationBar);
    if (!d->_pageNewWindowCountMap.contains(nodeKey))
    {
        return 0;
    }
    return d->_pageNewWindowCountMap[nodeKey];
}

const QList<ElaSuggestBox::SuggestData>& ElaNavigationBar::getSuggestDataList() const
{
    Q_D(const ElaNavigationBar);
    return d->_suggestDataList;
}

void ElaNavigationBar::paintEvent(QPaintEvent* event)
{
    Q_D(ElaNavigationBar);
    if (!d->_pIsTransparent)
    {
        QPainter painter(this);
        painter.save();
        painter.setPen(ElaThemeColor(d->_themeMode, BasicBorder));
        painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(d->_themeMode, PopupBase), "Background"));
        QRect baseRect = this->rect();
        baseRect.adjust(-1, 0, -1, 0);
        QPainterPath path;
        path.moveTo(baseRect.topLeft());
        path.lineTo(QPoint(baseRect.right() - 8, baseRect.y()));
        path.arcTo(QRectF(baseRect.right() - 16, baseRect.y(), 16, 16), 90, -90);
        path.lineTo(QPoint(baseRect.right(), baseRect.bottom() - 8));
        path.arcTo(QRectF(baseRect.right() - 16, baseRect.bottom() - 16, 16, 16), 0, -90);
        path.lineTo(baseRect.bottomLeft());
        path.closeSubpath();
        painter.drawPath(path);
        painter.restore();
    }
    QWidget::paintEvent(event);
}
