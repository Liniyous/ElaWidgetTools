#include "ElaNavigationBar.h"

#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScroller>
#include <QVBoxLayout>

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
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationBar, bool, IsTransparent)
ElaNavigationBar::ElaNavigationBar(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaNavigationBarPrivate())
{
    Q_D(ElaNavigationBar);
    d->q_ptr = this;
    setFixedWidth(300);
    d->_pIsTransparent = true;

    //用户卡片
    d->_userCard = new ElaInteractiveCard(this);
    connect(d->_userCard, &ElaInteractiveCard::clicked, this, &ElaNavigationBar::userInfoCardClicked);
    d->_userButton = new ElaIconButton(QPixmap(), this);
    d->_userButton->setFixedSize(36, 36);
    d->_userButton->setVisible(false);
    d->_userButton->setBorderRadius(8);
    d->_userButtonLayout = new QVBoxLayout();
    d->_userButtonLayout->setAlignment(Qt::AlignLeft);
    d->_userButtonLayout->setContentsMargins(0, 0, 0, 6);
    d->_userButtonLayout->setSpacing(0);
    d->_userButtonLayout->addWidget(d->_userButton);

    connect(d->_userButton, &ElaIconButton::clicked, this, &ElaNavigationBar::userInfoCardClicked);
    QHBoxLayout* userCardLayout = new QHBoxLayout();
    userCardLayout->setContentsMargins(0, 0, 0, 0);
    userCardLayout->addSpacing(3);
    userCardLayout->addLayout(d->_userButtonLayout);
    userCardLayout->addWidget(d->_userCard);

    // 搜索栏和按钮组
    d->_navigationButton = new ElaToolButton(this);
    d->_navigationButton->setFixedSize(40, 38);
    d->_navigationButton->setElaIcon(ElaIconType::Bars);
    d->_navigationButton->setBorderRadius(8);
    connect(d->_navigationButton, &ElaToolButton::clicked, d, &ElaNavigationBarPrivate::onNavigationButtonClicked);

    d->_searchButton = new ElaToolButton(this);
    d->_searchButton->setFixedSize(40, 38);
    d->_searchButton->setElaIcon(ElaIconType::MagnifyingGlass);
    d->_searchButton->setBorderRadius(8);
    connect(d->_searchButton, &ElaToolButton::clicked, d, &ElaNavigationBarPrivate::onNavigationButtonClicked);
    d->_searchButton->setVisible(false);

    d->_navigationSuggestBox = new ElaSuggestBox(this);
    d->_navigationSuggestBox->setMinimumWidth(0);
    d->_navigationButtonLayout = new QVBoxLayout();
    d->_navigationButtonLayout->setContentsMargins(0, 0, 0, 0);
    d->_navigationButtonLayout->setSpacing(0);
    d->_navigationButtonLayout->addWidget(d->_navigationButton);
    d->_navigationButtonLayout->addWidget(d->_searchButton);

    d->_navigationSuggestLayout = new QHBoxLayout();
    d->_navigationSuggestLayout->setContentsMargins(0, 0, 10, 0);
    d->_navigationSuggestLayout->setSpacing(6);
    d->_navigationSuggestLayout->addLayout(d->_navigationButtonLayout);
    d->_navigationSuggestLayout->addWidget(d->_navigationSuggestBox);

    // 搜索跳转
    connect(d->_navigationSuggestBox, &ElaSuggestBox::suggestionClicked, this, [=](QString suggestText, QVariantMap suggestData) {
        ElaNavigationNode* node = nullptr;
        if (suggestData.value("ElaNodeType").toString() == "Stacked")
        {
            node = d->_navigationModel->getNavigationNode(suggestData.value("ElaPageKey").toString());
            if (node)
            {
                d->onTreeViewClicked(node->getModelIndex());
            }
        }
        else
        {
            node = d->_footerModel->getNavigationNode(suggestData.value("ElaPageKey").toString());
            if (node)
            {
                d->onFooterViewClicked(node->getModelIndex());
            }
        }
    });

    // 导航模型
    d->_navigationModel = new ElaNavigationModel(this);
    d->_navigationView = new ElaNavigationView(this);
    d->_navigationView->setModel(d->_navigationModel);
    connect(d->_navigationView, &ElaNavigationView::navigationClicked, this, [=](const QModelIndex& index) { d->onTreeViewClicked(index); });
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
    connect(d->_footerView, &ElaBaseListView::clicked, this, [=](const QModelIndex& index) { d->onFooterViewClicked(index); });

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 10, 5, 0);
    mainLayout->addLayout(userCardLayout);
    mainLayout->addSpacing(4);
    mainLayout->addLayout(d->_navigationSuggestLayout);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(d->_navigationView);
    mainLayout->addWidget(d->_footerView);

    //主题设置
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaNavigationBar::~ElaNavigationBar()
{
}

void ElaNavigationBar::setUserInfoCardVisible(bool isVisible)
{
    Q_D(ElaNavigationBar);
    d->_isShowUserCard = isVisible;
    d->_userCard->setVisible(isVisible);
    if (!isVisible)
    {
        d->_userButton->setVisible(false);
    }
}

void ElaNavigationBar::setUserInfoCardPixmap(QPixmap pix)
{
    Q_D(ElaNavigationBar);
    d->_userCard->setCardPixmap(pix);
    d->_userButton->setPixmap(pix.scaled(d->_userButton->width(), d->_userButton->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
    ElaNavigationType::NodeOperateReturnType returnType = d_ptr->_footerModel->addFooterNode(footerTitle, footerKey, page ? true : false, keyPoints, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        d_ptr->_addFooterPage(page, footerKey);
    }
    return returnType;
}

void ElaNavigationBar::setNodeKeyPoints(QString nodeKey, int keyPoints)
{
    Q_D(ElaNavigationBar);
    ElaNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node || node->getIsExpanderNode() || keyPoints < 0)
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
    if (!node || node->getIsExpanderNode())
    {
        return -1;
    }
    return node->getKeyPoints();
}

void ElaNavigationBar::navigation(QString pageKey, bool isLogClicked)
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
            d->onFooterViewClicked(node->getModelIndex(), isLogClicked);
        }
        else
        {
            if (!node->getIsExpanderNode())
            {
                d->onTreeViewClicked(node->getModelIndex(), isLogClicked);
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

void ElaNavigationBar::paintEvent(QPaintEvent* event)
{
    Q_D(ElaNavigationBar);
    if (!d->_pIsTransparent)
    {
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing);
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
