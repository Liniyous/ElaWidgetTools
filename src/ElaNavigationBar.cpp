#include "ElaNavigationBar.h"

#include <QEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScroller>
#include <QVBoxLayout>

#include "ElaBaseListView.h"
#include "ElaCompactDelegate.h"
#include "ElaCompactModel.h"
#include "ElaFooterDelegate.h"
#include "ElaFooterModel.h"
#include "ElaInteractiveCard.h"
#include "ElaMenu.h"
#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationView.h"
#include "ElaSuggestBox.h"
#include "ElaTheme.h"
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
    d->_windowLinearGradient = new QLinearGradient(0, 0, width(), height());
    d->_windowLinearGradient->setColorAt(0, ElaThemeColor(ElaThemeType::Light, NavigationBaseStart));
    d->_windowLinearGradient->setColorAt(1, ElaThemeColor(ElaThemeType::Light, NavigationBaseEnd));

    d->_navigationModel = new ElaNavigationModel(this);
    d->_navigationView = new ElaNavigationView(this);
    d->_navigationView->setModel(d->_navigationModel);
    connect(d->_navigationView, &ElaNavigationView::navigationClicked, this, [=](const QModelIndex& index) { d->onTreeViewClicked(index); });

    d->_navigationSuggestBox = new ElaSuggestBox(this);
    d->_navigationSuggestBox->setMaximumWidth(300);
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

    d->_userCard = new ElaInteractiveCard(this);
    d->_userCard->setCardPixmap(QPixmap(":/include/Image/Cirno.jpg"));
    d->_userCard->setTitle("Ela Tool");
    d->_userCard->setSubTitle("Liniyous@gmail.com");
    connect(d->_userCard, &ElaInteractiveCard::clicked, this, &ElaNavigationBar::userInfoCardClicked);

    // 页脚
    d->_footerView = new ElaBaseListView(this);
    d->_footerView->setFixedHeight(0);
    d->_footerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_footerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_footerModel = new ElaFooterModel(this);
    d->_footerView->setModel(d->_footerModel);
    d->_footerDelegate = new ElaFooterDelegate(this);
    d->_footerDelegate->setElaListView(d->_footerView);
    d->_footerView->setItemDelegate(d->_footerDelegate);
    connect(d->_footerView, &ElaBaseListView::clicked, this, [=](const QModelIndex& index) { d->onFooterViewClicked(index); });

    //Maximal导航栏
    d->_maximalWidget = new QWidget(this);
    QVBoxLayout* maximalLayout = new QVBoxLayout(d->_maximalWidget);
    maximalLayout->setContentsMargins(5, 10, 10, 0);
    maximalLayout->addWidget(d->_userCard);
    maximalLayout->addSpacing(6);
    maximalLayout->addWidget(d->_navigationSuggestBox);
    maximalLayout->addSpacing(6);
    maximalLayout->addWidget(d->_navigationView);
    maximalLayout->addWidget(d->_footerView);

    //compact
    d->_compactWidget = new QWidget(this);
    QVBoxLayout* compactLayout = new QVBoxLayout(d->_compactWidget);
    compactLayout->setContentsMargins(0, 10, 0, 0);

    d->_compactView = new ElaBaseListView(this);
    d->_compactView->setFixedWidth(40);
    d->_compactView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_compactView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_compactModel = new ElaCompactModel(this);
    d->_compactView->setModel(d->_compactModel);
    d->_compactDelegate = new ElaCompactDelegate(this);
    d->_compactDelegate->setElaListView(d->_compactView);
    d->_compactView->setItemDelegate(d->_compactDelegate);
    compactLayout->addWidget(d->_compactView);
    connect(d->_compactView, &ElaBaseListView::clicked, this, [=](const QModelIndex& index) { d->onCompactViewClicked(index); });
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
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaNavigationBarPrivate::onThemeChanged);
}

ElaNavigationBar::~ElaNavigationBar()
{
}

void ElaNavigationBar::setUserInfoCardVisible(bool isVisible)
{
    Q_D(ElaNavigationBar);
    d->_userCard->setVisible(isVisible);
}

void ElaNavigationBar::setUserInfoCardPixmap(QPixmap pix)
{
    Q_D(ElaNavigationBar);
    d->_userCard->setCardPixmap(pix);
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
        ElaNavigationNode* node = d_ptr->_navigationModel->getNavigationNode(expanderKey);
        d->_compactModel->addCompactNode(node);
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
        ElaNavigationNode* node = d_ptr->_navigationModel->getNavigationNode(pageKey);
        d_ptr->_compactModel->addCompactNode(node);
        if (!d_ptr->_compactModel->getSelectedNode())
        {
            d_ptr->_compactModel->setSelectedNode(d_ptr->_navigationModel->getSelectedNode()->getOriginalNode());
        }
        d_ptr->_addStackedPage(page, pageKey);
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
    ElaNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, awesome);
    if (returnType == ElaNavigationType::NodeOperateReturnType::Success)
    {
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
        ElaNavigationNode* node = d_ptr->_navigationModel->getNavigationNode(pageKey);
        d_ptr->_compactModel->addCompactNode(node);
        if (!d_ptr->_compactModel->getSelectedNode())
        {
            d_ptr->_compactModel->setSelectedNode(d_ptr->_navigationModel->getSelectedNode()->getOriginalNode());
        }
        d_ptr->_addStackedPage(page, pageKey);
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
        ElaNavigationNode* node = d_ptr->_footerModel->getNavigationNode(footerKey);
        d_ptr->_compactModel->addCompactNode(node);
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
    ElaNavigationType::NavigationDisplayMode lastMode = d->_currentDisplayMode;
    d->_raiseNavigationBar();
    switch (displayMode)
    {
    case ElaNavigationType::Minimal:
    {
        QPropertyAnimation* navigationBarWidthAnimation = new QPropertyAnimation(this, "maximumWidth");
        connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            setFixedWidth(value.toUInt());
        });
        navigationBarWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
        navigationBarWidthAnimation->setStartValue(width());
        navigationBarWidthAnimation->setEndValue(0);
        navigationBarWidthAnimation->setDuration(isAnimation ? 300 : 0);
        navigationBarWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        while (d->_mainLayout->count() > 0)
        {
            d->_mainLayout->takeAt(0);
        }
        d->_compactWidget->resize(40, height());
        d->_compactWidget->setVisible(true);
        d->_startContentWidgetAnimation(d->_compactWidget->pos(), QPoint(-45, d->_compactWidget->pos().y()), isAnimation);
        break;
    }
    case ElaNavigationType::Compact:
    {
        QPropertyAnimation* navigationBarWidthAnimation = new QPropertyAnimation(this, "maximumWidth");
        connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            setFixedWidth(value.toUInt());
        });
        connect(navigationBarWidthAnimation, &QPropertyAnimation::finished, this, [=]() {
            d->_switchContentLayout(true);
        });
        navigationBarWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
        navigationBarWidthAnimation->setStartValue(width());
        navigationBarWidthAnimation->setEndValue(45);
        navigationBarWidthAnimation->setDuration(isAnimation ? 300 : 0);
        navigationBarWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        if (lastMode == ElaNavigationType::Maximal)
        {
            while (d->_mainLayout->count() > 0)
            {
                d->_mainLayout->takeAt(0);
            }
            d->_compactWidget->resize(40, height());
            d->_compactWidget->setVisible(true);
            d->_startContentWidgetAnimation(QPoint(width(), d->_compactWidget->pos().y()), QPoint(0, d->_compactWidget->pos().y()), isAnimation);
        }
        else
        {
            d->_resetNavigationLayout(ElaNavigationType::Minimal);
            d->_startContentWidgetAnimation(d->_compactWidget->pos(), QPoint(0, d->_compactWidget->pos().y()), isAnimation);
        }
        break;
    }
    case ElaNavigationType::Maximal:
    {
        QPropertyAnimation* navigationBarWidthAnimation = new QPropertyAnimation(this, "maximumWidth");
        connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            setFixedWidth(value.toUInt());
        });
        connect(navigationBarWidthAnimation, &QPropertyAnimation::finished, this, [=]() {
            d->_switchContentLayout(false);
        });
        navigationBarWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
        navigationBarWidthAnimation->setStartValue(width());
        navigationBarWidthAnimation->setEndValue(300);
        navigationBarWidthAnimation->setDuration(isAnimation ? 300 : 0);
        navigationBarWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        while (d->_mainLayout->count() > 0)
        {
            d->_mainLayout->takeAt(0);
        }
        d->_maximalWidget->resize(300, height());
        d->_maximalWidget->setVisible(true);
        d->_startContentWidgetAnimation(d->_compactWidget->pos(), QPoint(300, d->_compactWidget->pos().y()), isAnimation);
        break;
    }
    default:
    {
        return;
    }
    }
    d->_currentDisplayMode = displayMode;
}

void ElaNavigationBar::paintEvent(QPaintEvent* event)
{
    Q_D(ElaNavigationBar);
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    if (d->_pIsTransparent)
    {
        painter.setBrush(Qt::transparent);
    }
    else
    {
        painter.setBrush(*d->_windowLinearGradient);
    }
    painter.drawRoundedRect(rect(), 8, 8);
    painter.restore();
    QWidget::paintEvent(event);
}

void ElaNavigationBar::resizeEvent(QResizeEvent* event)
{
    Q_D(ElaNavigationBar);
    if (d->_currentDisplayMode == ElaNavigationType::Compact)
    {
        d->_compactWidget->resize(d->_compactWidget->width(), this->height());
    }
    QWidget::resizeEvent(event);
}
