#include "ElaNavigationBar.h"

#include "ElaBaseListView.h"
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
#include "private/ElaSuggestBoxPrivate.h"
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScroller>
#include <QVBoxLayout>
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationBar, bool, IsTransparent)
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationBar, bool, IsAllowPageOpenInNewWindow)
ElaNavigationBar::ElaNavigationBar(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaNavigationBarPrivate())
{
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
    d->_userCard->setTitle("Ela Tool");
    d->_userCard->setSubTitle("Liniyous@gmail.com");
    connect(d->_userCard, &ElaInteractiveCard::clicked, this, &ElaNavigationBar::userInfoCardClicked);
    d->_userButton = new ElaIconButton(QPixmap(":/include/Image/Cirno.jpg"), this);
    d->_userButton->setFixedSize(36, 36);
    d->_userButton->setVisible(false);
    d->_userButton->setBorderRadius(8);
    d->_userButtonLayout = new QVBoxLayout();
    d->_userButtonLayout->setAlignment(Qt::AlignLeft);
    d->_userButtonLayout->setContentsMargins(0, 0, 0, 0);
    d->_userButtonLayout->setSpacing(0);
    d->_userButtonLayout->addWidget(d->_userButton);

    connect(d->_userButton, &ElaIconButton::clicked, this, &ElaNavigationBar::userInfoCardClicked);
    d->_userCardLayout = new QVBoxLayout();
    d->_userCardLayout->setContentsMargins(3, 10, 0, 5);
    d->_userCardLayout->addLayout(d->_userButtonLayout);
    d->_userCardLayout->addWidget(d->_userCard);

    // 搜索跳转
    //    connect(d->_navigationSuggestBox, &ElaSuggestBox::suggestionClicked, this, [=](QString suggestText, QVariantMap suggestData) {
    //        navigation(suggestData.value("ElaPageKey").toString());
    //    });

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
    d->_footerDelegate = new ElaFooterDelegate(this);
    d->_footerDelegate->setElaListView(d->_footerView);
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
    mainLayout->addLayout(d->_userCardLayout);
    mainLayout->addWidget(d->_navigationView);
    mainLayout->addWidget(d->_footerView);

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
    if (d->_currentDisplayMode == ElaNavigationType::NavigationDisplayMode::Maximal)
    {
        setFixedWidth(navigationBarWidth);
    }
    d->_pNavigationBarWidth = navigationBarWidth;
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
    d->_userCard->setVisible(isVisible);
    if (isVisible)
    {
        d->_userCardLayout->setContentsMargins(3, 10, 5, 5);
    }
    else
    {
        d->_userCardLayout->setContentsMargins(0, 0, 0, 0);
        d->_userButton->setVisible(false);
    }
}

void ElaNavigationBar::setUserInfoCardPixmap(QPixmap pix)
{
    Q_D(ElaNavigationBar);
    d->_userCard->setCardPixmap(pix);
    d->_userButton->setPixmap(pix);
}

void ElaNavigationBar::setUserInfoCardTitle(QString title)
{
    Q_D(ElaNavigationBar);
    d->_userCard->setTitle(title);
}

void ElaNavigationBar::setUserInfoCardSubTitle(QString subTitle)
{
    Q_D(ElaNavigationBar);
    d->_userCard->setSubTitle(subTitle);
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType::IconName awesome)
{
    Q_D(ElaNavigationBar);
    ElaNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addExpanderNode(expanderTitle, expanderKey, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType::IconName awesome)
{
    Q_D(ElaNavigationBar);
    ElaNavigationType::NodeOperateReturnType returnType = d->_navigationModel->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addPageNode(QString pageTitle, QWidget* page, ElaIconType::IconName awesome)
{
    Q_D(ElaNavigationBar);
    if (!page)
    {
        return ElaNavigationType::PageInvalid;
    }
    QString pageKey;
    ElaNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_pageNewWindowCountMap.insert(pageKey, 0);
        d->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, ElaIconType::IconName awesome)
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
    ElaNavigationType::NodeOperateReturnType returnType = d->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, awesome);
    if (returnType == ElaNavigationType::NodeOperateReturnType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_pageNewWindowCountMap.insert(pageKey, 0);
        ElaNavigationNode* node = d->_navigationModel->getNavigationNode(pageKey);
        ElaNavigationNode* originalNode = node->getOriginalNode();
        if (d->_compactMenuMap.contains(originalNode))
        {
            ElaMenu* menu = d->_compactMenuMap.value(originalNode);
            QAction* action = menu->addElaIconAction(node->getAwesome(), node->getNodeTitle());
            connect(action, &QAction::triggered, this, [=]() {
                d->onTreeViewClicked(node->getModelIndex());
            });
        }
        else
        {
            ElaMenu* menu = new ElaMenu(const_cast<ElaNavigationBar*>(this));
            QAction* action = menu->addElaIconAction(node->getAwesome(), node->getNodeTitle());
            connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
            d_ptr->_compactMenuMap.insert(originalNode, menu);
        }
        d_ptr->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addPageNode(QString pageTitle, QWidget* page, int keyPoints, ElaIconType::IconName awesome)
{
    Q_D(ElaNavigationBar);
    if (!page)
    {
        return ElaNavigationType::PageInvalid;
    }
    QString pageKey;
    ElaNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, keyPoints, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_pageNewWindowCountMap.insert(pageKey, 0);
        d->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints, ElaIconType::IconName awesome)
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
    ElaNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, keyPoints, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_pageNewWindowCountMap.insert(pageKey, 0);
        ElaNavigationNode* node = d_ptr->_navigationModel->getNavigationNode(pageKey);
        ElaNavigationNode* originalNode = node->getOriginalNode();
        if (d_ptr->_compactMenuMap.contains(originalNode))
        {
            ElaMenu* menu = d_ptr->_compactMenuMap.value(originalNode);
            QAction* action = menu->addElaIconAction(node->getAwesome(), node->getNodeTitle());
            connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
        }
        else
        {
            ElaMenu* menu = new ElaMenu(const_cast<ElaNavigationBar*>(this));
            QAction* action = menu->addElaIconAction(node->getAwesome(), node->getNodeTitle());
            connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
            d_ptr->_compactMenuMap.insert(originalNode, menu);
        }
        d_ptr->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, ElaIconType::IconName awesome)
{
    return addFooterNode(footerTitle, nullptr, footerKey, keyPoints, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints, ElaIconType::IconName awesome)
{
    ElaNavigationType::NodeOperateReturnType returnType = d_ptr->_footerModel->addFooterNode(footerTitle, footerKey, page != nullptr, keyPoints, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        d_ptr->_addFooterPage(page, footerKey);
    }
    return returnType;
}

bool ElaNavigationBar::getNodeIsExpanded(QString expanderKey) const
{
    Q_D(const ElaNavigationBar);
    ElaNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode())
    {
        return false;
    }
    return d->_navigationView->isExpanded(node->getModelIndex());
}

void ElaNavigationBar::expandNode(QString expanderKey)
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

void ElaNavigationBar::collapseNode(QString expanderKey)
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

void ElaNavigationBar::removeNode(QString nodeKey)
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
    if (node->getIsFooterNode())
    {
        Q_EMIT navigationNodeRemoved(ElaNavigationType::FooterNode, nodeKey);
        d->_footerModel->removeNavigationNode(nodeKey);
        d->_footerView->setFixedHeight(40 * d->_footerModel->getFooterNodeCount());
    }
    else
    {
        QStringList removeKeyList = d->_navigationModel->removeNavigationNode(nodeKey);
        d->_initNodeModelIndex(QModelIndex());
        for (const auto& removeKey: removeKeyList)
        {
            d->_pageMetaMap.remove(removeKey);
            d->_pageNewWindowCountMap.remove(removeKey);
            Q_EMIT navigationNodeRemoved(ElaNavigationType::PageNode, removeKey);
        }
    }
    for (int i = 0; i < d->_suggestDataList.count(); i++)
    {
        auto& suggestData = d->_suggestDataList[i];
        if (suggestData.getSuggestData().value("ElaPageKey").toString() == nodeKey)
        {
            d->_suggestDataList.removeAt(i);
            break;
        }
    }
}

void ElaNavigationBar::setNodeKeyPoints(QString nodeKey, int keyPoints)
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

int ElaNavigationBar::getNodeKeyPoints(QString nodeKey) const
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

void ElaNavigationBar::setNodeTitle(QString nodeKey, QString nodeTitle)
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

QString ElaNavigationBar::getNodeTitle(QString nodeKey) const
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

void ElaNavigationBar::navigation(QString pageKey, bool isLogClicked, bool isRouteBack)
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
                d->onTreeViewClicked(node->getModelIndex(), isLogClicked, isRouteBack);
                d->_smoothScrollNavigationView(node->getModelIndex());
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

int ElaNavigationBar::getPageOpenInNewWindowCount(QString nodeKey) const
{
    Q_D(const ElaNavigationBar);
    if (!d->_pageNewWindowCountMap.contains(nodeKey))
    {
        return 0;
    }
    return d->_pageNewWindowCountMap[nodeKey];
}

QList<ElaSuggestBox::SuggestData> ElaNavigationBar::getSuggestDataList() const
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
        painter.setPen(ElaThemeColor(d->_themeMode, PopupBorder));
        painter.setBrush(ElaThemeColor(d->_themeMode, PopupBase));
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
