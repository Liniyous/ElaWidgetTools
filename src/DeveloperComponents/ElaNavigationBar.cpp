#include "ElaNavigationBar.h"

#include <Windows.h>

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPropertyAnimation>
#include <QScroller>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "ElaApplication.h"
#include "ElaCompactDelegate.h"
#include "ElaCompactModel.h"
#include "ElaEventBus.h"
#include "ElaFooterDelegate.h"
#include "ElaFooterModel.h"
#include "ElaInteractiveCard.h"
#include "ElaListView.h"
#include "ElaMenu.h"
#include "ElaNavigationDelegate.h"
#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationSuggestBox.h"
#include "ElaNavigationSuggestView.h"
#include "ElaNavigationView.h"
#include "ElaWindow.h"
#include "ElaWindowPrivate.h"
#include "private/ElaNavigationBarPrivate.h"
#include "private/ElaNavigationSuggestBoxPrivate.h"

ElaNavigationBar::ElaNavigationBar(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaNavigationBarPrivate())
{
    Q_D(ElaNavigationBar);
    d->q_ptr = this;
    setFixedWidth(300);
    d->_windowLinearGradient = new QLinearGradient(0, 0, width(), height());
    d->_windowLinearGradient->setColorAt(0, QColor(0xF3, 0xF2, 0xF9));
    d->_windowLinearGradient->setColorAt(1, QColor(0xF4, 0xF1, 0xF8));

    d->_navigationModel = new ElaNavigationModel(this);
    d->_navigationView = new ElaNavigationView(this);
    d->_navigationView->setModel(d->_navigationModel);
    d->_navigationDelegate = new ElaNavigationDelegate(this);
    d->_navigationDelegate->setNavigationView(d->_navigationView);
    d->_navigationView->setItemDelegateForColumn(0, d->_navigationDelegate);
    connect(d->_navigationView, &ElaNavigationView::navigationClicked, this, [=](const QModelIndex& index) { d->onTreeViewClicked(index); });

    d->_navigationSuggestBox = new ElaNavigationSuggestBox(this);
    d->_navigationSuggestBox->setFixedSize(280, 35);
    // 搜索跳转
    connect(d->_navigationSuggestBox, &ElaNavigationSuggestBox::searchNodeClicked, this, [=](ElaNavigationNode* node, bool isLogClicked = true) {
        if (node->getIsFooterNode())
        {
            d->onFooterViewClicked(node->getModelIndex());
        }
        else
        {
            d->onTreeViewClicked(node->getModelIndex());
        }
    });

    d->_userCard = new ElaInteractiveCard(this);
    d->_userCard->setCardPixmap(QPixmap(":/include/Image/Cirno.jpg"));
    d->_userCard->setTitle("Ela Tool");
    d->_userCard->setSubTitle("Liniyous@gmail.com");
    ElaWindow* window = dynamic_cast<ElaWindow*>(parent);
    connect(d->_userCard, &ElaInteractiveCard::clicked, this, [=]() { Q_EMIT window->userInfoCardClicked(); });

    // 页脚
    d->_footerView = new ElaListView(this);
    d->_footerView->setFixedHeight(0);
    d->_footerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_footerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_footerModel = new ElaFooterModel(this);
    d->_footerView->setModel(d->_footerModel);
    d->_footerDelegate = new ElaFooterDelegate(this);
    d->_footerDelegate->setElaListView(d->_footerView);
    d->_footerView->setItemDelegate(d->_footerDelegate);
    connect(d->_footerView, &ElaListView::clicked, this, [=](const QModelIndex& index) { d->onFooterViewClicked(index); });

    //Maximal导航栏
    d->_maximalWidget = new QWidget(this);
    QVBoxLayout* maximalLayout = new QVBoxLayout(d->_maximalWidget);
    maximalLayout->setContentsMargins(10, 10, 10, 0);
    maximalLayout->addWidget(d->_userCard);
    maximalLayout->addSpacing(6);
    maximalLayout->addWidget(d->_navigationSuggestBox);
    maximalLayout->addSpacing(12);
    maximalLayout->addWidget(d->_navigationView);
    maximalLayout->addWidget(d->_footerView);

    //compact
    d->_compactWidget = new QWidget(this);
    QVBoxLayout* compactLayout = new QVBoxLayout(d->_compactWidget);
    compactLayout->setContentsMargins(0, 10, 0, 0);

    d->_compactView = new ElaListView(this);
    d->_compactView->setFixedWidth(40);
    d->_compactView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_compactView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_compactModel = new ElaCompactModel(this);
    d->_compactView->setModel(d->_compactModel);
    d->_compactDelegate = new ElaCompactDelegate(this);
    d->_compactDelegate->setElaListView(d->_compactView);
    d->_compactView->setItemDelegate(d->_compactDelegate);
    compactLayout->addWidget(d->_compactView);
    connect(d->_compactView, &ElaListView::clicked, this, [=](const QModelIndex& index) { d->onCompactViewClicked(index); });
    QScroller::grabGesture(d->_compactView->viewport(), QScroller::LeftMouseButtonGesture);
    QScrollerProperties properties = QScroller::scroller(d->_compactView->viewport())->scrollerProperties();
#if (QT_VERSION < QT_VERSION_CHECK(6, 5, 3))
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
#endif
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    QScroller::scroller(d->_compactView->viewport())->setScrollerProperties(properties);

    d->_mainLayout = new QHBoxLayout(this);
    d->_mainLayout->setAlignment(Qt::AlignLeft);
    d->_mainLayout->setSpacing(0);
    d->_mainLayout->setContentsMargins(0, 0, 0, 0);
    d->_mainLayout->addWidget(d->_maximalWidget);
    d->_mainLayout->addWidget(d->_compactWidget);
    d->_compactWidget->setVisible(false);

    //主题设置
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, d, &ElaNavigationBarPrivate::onThemeChanged);
}

ElaNavigationBar::~ElaNavigationBar()
{
}

void ElaNavigationBar::setUserInfoCardVisible(bool isVisible)
{
    Q_D(ElaNavigationBar);
    d->_userCard->setVisible(isVisible);
    if (isVisible)
    {
        d->_navigationSuggestBox->d_ptr->_searchView->move(16, 174);
    }
    else
    {
        d->_navigationSuggestBox->d_ptr->_searchView->move(16, 86);
    }
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType awesome) const
{
    ElaNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addExpanderNode(expanderTitle, expanderKey, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        ElaNavigationNode* node = d_ptr->_navigationModel->getNavigationNode(expanderKey);
        d_ptr->_compactModel->addCompactNode(node);
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType awesome) const
{
    return d_ptr->_navigationModel->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addPageNode(QString pageTitle, QWidget* page, ElaIconType awesome) const
{
    if (!page)
    {
        return ElaNavigationType::PageInvalid;
    }
    QString pageKey;
    ElaNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        ElaNavigationNode* node = d_ptr->_navigationModel->getNavigationNode(pageKey);
        d_ptr->_compactModel->addCompactNode(node);
        if (!d_ptr->_compactModel->getSelectedNode())
        {
            d_ptr->_compactModel->setSelectedNode(d_ptr->_navigationModel->getSelectedNode()->getOriginalNode());
        }
        d_ptr->_addStackedPage(page, pageKey);
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, ElaIconType awesome) const
{
    if (!page)
    {
        return ElaNavigationType::PageInvalid;
    }
    if (targetExpanderKey.isEmpty())
    {
        return ElaNavigationType::TargetNodeInvalid;
    }
    QString pageKey;
    ElaNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, awesome);
    if (returnType == ElaNavigationType::NodeOperateReturnType::Success)
    {
        ElaNavigationNode* node = d_ptr->_navigationModel->getNavigationNode(pageKey);
        ElaNavigationNode* originalNode = node->getOriginalNode();
        if (d_ptr->_compactMenuMap.contains(originalNode))
        {
            ElaMenu* menu = d_ptr->_compactMenuMap.value(originalNode);
            QAction* action = menu->addAction(node->getAwesome(), node->getNodeTitle());
            connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
        }
        else
        {
            ElaMenu* menu = new ElaMenu(const_cast<ElaNavigationBar*>(this));
            QAction* action = menu->addAction(node->getAwesome(), node->getNodeTitle());
            connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
            d_ptr->_compactMenuMap.insert(originalNode, menu);
        }
        d_ptr->_addStackedPage(page, pageKey);
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addPageNode(QString pageTitle, QWidget* page, int keyPoints, ElaIconType awesome) const
{
    if (!page)
    {
        return ElaNavigationType::PageInvalid;
    }
    QString pageKey;
    ElaNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, keyPoints, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        ElaNavigationNode* node = d_ptr->_navigationModel->getNavigationNode(pageKey);
        d_ptr->_compactModel->addCompactNode(node);
        if (!d_ptr->_compactModel->getSelectedNode())
        {
            d_ptr->_compactModel->setSelectedNode(d_ptr->_navigationModel->getSelectedNode()->getOriginalNode());
        }
        d_ptr->_addStackedPage(page, pageKey);
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints, ElaIconType awesome) const
{
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
        ElaNavigationNode* node = d_ptr->_navigationModel->getNavigationNode(pageKey);
        ElaNavigationNode* originalNode = node->getOriginalNode();
        if (d_ptr->_compactMenuMap.contains(originalNode))
        {
            ElaMenu* menu = d_ptr->_compactMenuMap.value(originalNode);
            QAction* action = menu->addAction(node->getAwesome(), node->getNodeTitle());
            connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
        }
        else
        {
            ElaMenu* menu = new ElaMenu(const_cast<ElaNavigationBar*>(this));
            QAction* action = menu->addAction(node->getAwesome(), node->getNodeTitle());
            connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
            d_ptr->_compactMenuMap.insert(originalNode, menu);
        }
        d_ptr->_addStackedPage(page, pageKey);
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaNavigationBar::addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints, ElaIconType awesome) const
{
    ElaNavigationType::NodeOperateReturnType returnType = d_ptr->_footerModel->addFooterNode(footerTitle, footerKey, page ? true : false, keyPoints, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        ElaNavigationNode* node = d_ptr->_footerModel->getNavigationNode(footerKey);
        d_ptr->_compactModel->addCompactNode(node);
        d_ptr->_addFooterPage(page, footerKey);
    }
    return returnType;
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

void ElaNavigationBar::switchCompact(bool direction)
{
    Q_D(ElaNavigationBar);
    if (direction)
    {
        setFixedWidth(45);
        while (d->_mainLayout->count() > 0)
        {
            d->_mainLayout->takeAt(0);
        }
        d->_mainLayout->addWidget(d->_compactWidget);
        d->_maximalWidget->setVisible(false);
        d->_compactWidget->setVisible(true);
        move(-45, this->pos().y());
    }
    else
    {
        setFixedWidth(300);
        while (d->_mainLayout->count() > 0)
        {
            d->_mainLayout->takeAt(0);
        }
        d->_mainLayout->addWidget(d->_maximalWidget);
        d->_maximalWidget->setVisible(true);
        d->_compactWidget->setVisible(false);
    }
}

void ElaNavigationBar::paintEvent(QPaintEvent* event)
{
    Q_D(ElaNavigationBar);
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(*d->_windowLinearGradient);
    painter.drawRoundedRect(rect(), 8, 8);
    painter.restore();
    QWidget::paintEvent(event);
}
