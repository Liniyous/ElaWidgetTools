#include "ElaWindow.h"

#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "DeveloperComponents/private/ElaNavigationBarPrivate.h"
#include "ElaAppBar.h"
#include "ElaApplication.h"
#include "ElaEventBus.h"
#include "ElaNavigationBar.h"
#include "ElaNavigationRouter.h"
#include "private/ElaAppBarPrivate.h"
#include "private/ElaWindowPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaWindow, int, ThemeChangeTime)
Q_PROPERTY_CREATE_Q_CPP(ElaWindow, ElaNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)

ElaWindow::ElaWindow(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaWindowPrivate())
{
    Q_D(ElaWindow);
    d->q_ptr = this;
    setProperty("ElaBaseClassName", "ElaWindow");
    resize(1020, 680); // 默认宽高

    d->_pThemeChangeTime = 700;
    d->_pNavigationBarDisplayMode = ElaNavigationType::NavigationDisplayMode::Auto;
    connect(this, &ElaWindow::pNavigationBarDisplayModeChanged, d, &ElaWindowPrivate::onDisplayModeChanged);

    d->_windowLinearGradient = new QLinearGradient(0, 0, width(), height());
    d->_windowLinearGradient->setColorAt(0, QColor(0xF2, 0xF2, 0xF9));
    d->_windowLinearGradient->setColorAt(1, QColor(0xF9, 0xEF, 0xF6));
    // 自定义AppBar
    d->_appBar = new ElaAppBar(this);
    connect(d->_appBar, &ElaAppBar::routeBackButtonClicked, this, []() {
        ElaNavigationRouter::getInstance()->navigationRouteBack();
    });
    connect(d->_appBar, &ElaAppBar::closeButtonClicked, this, &ElaWindow::closeButtonClicked);

    // 导航栏
    d->_navigationBar = new ElaNavigationBar(this);
    // 返回按钮状态变更
    connect(ElaNavigationRouter::getInstance(), &ElaNavigationRouter::navigationRouterStateChanged, this, [d](bool isEnable) {
        d->_appBar->setRouteBackButtonEnable(isEnable);
    });

    // 转发用户卡片点击信号
    connect(d->_navigationBar, &ElaNavigationBar::userInfoCardClicked, this, &ElaWindow::userInfoCardClicked);
    // 转发点击信号
    connect(d->_navigationBar, &ElaNavigationBar::navigationNodeClicked, this, &ElaWindow::navigationNodeClicked);
    //跳转处理
    connect(d->_navigationBar, &ElaNavigationBar::navigationNodeClicked, d, &ElaWindowPrivate::onNavigationNodeClicked);
    //新增窗口
    connect(d->_navigationBar, &ElaNavigationBar::navigationNodeAdded, d, &ElaWindowPrivate::onNavigationNodeAdded);

    // 中心堆栈窗口
    d->_centerStackedWidget = new QStackedWidget(this);
    d->_centerStackedWidget->setContentsMargins(0, 10, 0, 0);
    d->_mainLayout = new QVBoxLayout(this);
    d->_mainLayout->setSpacing(0);
    d->_mainLayout->setContentsMargins(0, 0, 0, 0);
    d->_centerLayout = new QHBoxLayout();
    d->_centerLayout->addWidget(d->_navigationBar);
    d->_centerLayout->addWidget(d->_centerStackedWidget);
    int contentMargin = d->_contentsMargins;
    d->_centerLayout->setContentsMargins(contentMargin, contentMargin, contentMargin, contentMargin);
    d->_mainLayout->addWidget(d->_appBar);
    d->_mainLayout->addLayout(d->_centerLayout);

    // 事件总线
    d->_focusEvent = new ElaEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();

    // 展开导航栏
    connect(d->_appBar, &ElaAppBar::navigationButtonClicked, d, &ElaWindowPrivate::onNavigationButtonClicked);

    // 主题变更动画
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, d, &ElaWindowPrivate::onThemeModeChanged);
    connect(d->_appBar, &ElaAppBar::themeChangeButtonClicked, d, &ElaWindowPrivate::onThemeReadyChange);
}

ElaWindow::~ElaWindow()
{
}

void ElaWindow::setIsNavigationBarEnable(bool isVisible)
{
    Q_D(ElaWindow);
    d->_isNavigationEnable = isVisible;
    d->_navigationBar->setVisible(isVisible);
}

bool ElaWindow::getIsNavigationBarEnable() const
{
    return d_ptr->_isNavigationEnable;
}

void ElaWindow::setWindowTitle(QString title)
{
    Q_D(ElaWindow);
    d->_appBar->setWindowTitle(title);
}

void ElaWindow::setIsStayTop(bool isStayTop)
{
    Q_D(ElaWindow);
    d->_appBar->setIsStayTop(isStayTop);
}

bool ElaWindow::getIsStayTop() const
{
    return d_ptr->_appBar->getIsStayTop();
}

void ElaWindow::setIsFixedSize(bool isFixedSize)
{
    Q_D(ElaWindow);
    d->_appBar->setIsFixedSize(isFixedSize);
}

bool ElaWindow::getIsFixedSize() const
{
    return d_ptr->_appBar->getIsFixedSize();
}

void ElaWindow::setUserInfoCardVisible(bool isVisible)
{
    Q_D(ElaWindow);
    d->_navigationBar->setUserInfoCardVisible(isVisible);
}

void ElaWindow::setUserInfoCardPixmap(QPixmap pix)
{
    Q_D(ElaWindow);
    d->_navigationBar->setUserInfoCardPixmap(pix);
}

void ElaWindow::setUserInfoCardTitle(QString title)
{
    Q_D(ElaWindow);
    d->_navigationBar->setUserInfoCardTitle(title);
}

void ElaWindow::setUserInfoCardSubTitle(QString subTitle)
{
    Q_D(ElaWindow);
    d->_navigationBar->setUserInfoCardSubTitle(subTitle);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addExpanderNode(expanderTitle, expanderKey, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, ElaIconType awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addPageNode(pageTitle, page, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, ElaIconType awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, int keyPoints, ElaIconType awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addPageNode(pageTitle, page, keyPoints, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints, ElaIconType awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, keyPoints, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, ElaIconType awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addFooterNode(footerTitle, nullptr, footerKey, keyPoints, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints, ElaIconType awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addFooterNode(footerTitle, page, footerKey, keyPoints, awesome);
}

void ElaWindow::setNodeKeyPoints(QString nodeKey, int keyPoints)
{
    Q_D(ElaWindow);
    d->_navigationBar->setNodeKeyPoints(nodeKey, keyPoints);
}

int ElaWindow::getNodeKeyPoints(QString nodeKey) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->getNodeKeyPoints(nodeKey);
}

void ElaWindow::navigation(QString pageKey)
{
    Q_D(ElaWindow);
    d->_navigationBar->navigation(pageKey);
}

void ElaWindow::setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable)
{
    Q_D(ElaWindow);
    d->_appBar->setWindowButtonFlag(buttonFlag, isEnable);
}

void ElaWindow::setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags)
{
    Q_D(ElaWindow);
    d->_appBar->setWindowButtonFlags(buttonFlags);
}

ElaAppBarType::ButtonFlags ElaWindow::getWindowButtonFlags() const
{
    return d_ptr->_appBar->getWindowButtonFlags();
}

void ElaWindow::setIsDefaultClosed(bool isDefaultClosed)
{
    Q_D(ElaWindow);
    d->_appBar->setIsDefaultClosed(isDefaultClosed);
}

bool ElaWindow::getIsDefaultClosed() const
{
    return d_ptr->_appBar->getIsDefaultClosed();
}

void ElaWindow::closeWindow()
{
    Q_D(ElaWindow);
    d->_appBar->closeWindow();
}

void ElaWindow::paintEvent(QPaintEvent* event)
{
    Q_D(ElaWindow);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    d->_windowLinearGradient->setFinalStop(width(), height());
    painter.setBrush(*d->_windowLinearGradient);
    painter.drawRect(rect());
    painter.restore();
    QWidget::paintEvent(event);
}

void ElaWindow::resizeEvent(QResizeEvent* event)
{
    Q_D(ElaWindow);
    if (ElaApplication::getInstance()->getIsApplicationClosed() || !d->_isNavigationEnable)
    {
        return;
    }
    d->_windowLinearGradient->setFinalStop(width(), height());
    if (d->_pNavigationBarDisplayMode == ElaNavigationType::Minimal)
    {
        d->_resetWindowLayout(false);
    }
    if (d->_pNavigationBarDisplayMode == ElaNavigationType::Auto)
    {
        d->_resetWindowLayout(false);
        if (width() >= 850 && d->_currentNavigationBarDisplayMode != ElaNavigationType::Maximal)
        {
            d->_navigationBar->setDisplayMode(ElaNavigationType::Maximal);
            d->_isNavigationBarExpanded = false;
            d->_currentNavigationBarDisplayMode = ElaNavigationType::Maximal;
            d->_appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint, false);
        }
        else if (width() >= 550 && width() < 850 && d->_currentNavigationBarDisplayMode != ElaNavigationType::Compact)
        {
            d->_navigationBar->setDisplayMode(ElaNavigationType::Compact);
            d->_isNavigationBarExpanded = false;
            d->_currentNavigationBarDisplayMode = ElaNavigationType::Compact;
            d->_appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint, false);
        }
        else if (width() < 550 && d->_currentNavigationBarDisplayMode != ElaNavigationType::Minimal)
        {
            d->_navigationBar->setDisplayMode(ElaNavigationType::Minimal);
            d->_isNavigationBarExpanded = false;
            d->_currentNavigationBarDisplayMode = ElaNavigationType::Minimal;
            d->_appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint);
        }
    }
    QWidget::resizeEvent(event);
}
