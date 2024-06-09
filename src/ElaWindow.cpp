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

#include "DeveloperComponents/ElaNavigationBar.h"
#include "DeveloperComponents/ElaThemeAnimationWidget.h"
#include "DeveloperComponents/private/ElaNavigationBarPrivate.h"
#include "ElaAppBar.h"
#include "ElaApplication.h"
#include "ElaEventBus.h"
#include "ElaInteractiveCard.h"
#include "private/ElaAppBarPrivate.h"
#include "private/ElaWindowPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaWindow, int, ThemeChangeTime)
Q_PROPERTY_CREATE_Q_CPP(ElaWindow, ElaNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)

ElaWindow::ElaWindow(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaWindowPrivate())
{
    ElaApplication::getInstance()->init();
    setObjectName("ElaWidnow");
    Q_D(ElaWindow);
    resize(1020, 680); // 默认宽高
    d->q_ptr = this;
    d->_pThemeChangeTime = 700;
    d->_pNavigationBarDisplayMode = ElaNavigationType::NavigationDisplayMode::Auto;
    connect(this, &ElaWindow::pNavigationBarDisplayModeChanged, d, &ElaWindowPrivate::onDisplayModeChanged);

    d->_windowLinearGradient = new QLinearGradient(0, 0, width(), height());
    d->_windowLinearGradient->setColorAt(0, QColor(0xF2, 0xF2, 0xF9));
    d->_windowLinearGradient->setColorAt(1, QColor(0xF9, 0xEF, 0xF6));
    // 自定义AppBar
    d->_appBar = new ElaAppBar(this);
    connect(d->_appBar, &ElaAppBar::closeButtonClicked, this, &ElaWindow::closeButtonClicked);

    // 导航栏
    d->_navigationBar = new ElaNavigationBar(this);
    // 返回按钮状态变更
    connect(d->_navigationBar, &ElaNavigationBar::routeBackButtonStateChanged, this, [d](bool isEnable) {
        d->_appBar->setRouteBackButtonEnable(isEnable);
    });
    // 返回按钮按下
    connect(d->_appBar, &ElaAppBar::routeBackButtonClicked, d->_navigationBar->d_func(), &ElaNavigationBarPrivate::onRouteBackButtonClicked);
    // 页脚没有绑定页面时发送
    connect(d->_navigationBar, &ElaNavigationBar::footerNodeClicked, this, &ElaWindow::footerNodeClicked);

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
    d->_focusEvent = new ElaEvent("WMWindowClicked", this);
    ElaEventBus::getInstance()->registerEvent(d->_focusEvent);
    connect(d->_focusEvent, &ElaEvent::triggered, d, &ElaWindowPrivate::onWMWindowClickedEvent);

    // 展开导航栏
    connect(d->_appBar, &ElaAppBar::navigationButtonClicked, d, &ElaWindowPrivate::onNavigationButtonClicked);

    // 主题变更动画
    connect(d->_appBar, &ElaAppBar::themeChangeButtonClicked, d, &ElaWindowPrivate::onThemeReadyChange);
    // 主题变更绘制窗口
    d->_animationWidget = new ElaThemeAnimationWidget(this);
    d->_animationWidget->move(0, 0);
    d->_animationWidget->hide();
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
    d->_navigationBar->d_ptr->_userCard->setCardPixmap(pix);
}

void ElaWindow::setUserInfoCardTitle(QString title)
{
    Q_D(ElaWindow);
    d->_navigationBar->d_ptr->_userCard->setTitle(title);
}

void ElaWindow::setUserInfoCardSubTitle(QString subTitle)
{
    Q_D(ElaWindow);
    d->_navigationBar->d_ptr->_userCard->setSubTitle(subTitle);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType awesome) const
{
    return d_ptr->_navigationBar->addExpanderNode(expanderTitle, expanderKey, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType awesome) const
{
    return d_ptr->_navigationBar->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, ElaIconType awesome) const
{
    return d_ptr->_navigationBar->addPageNode(pageTitle, page, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, ElaIconType awesome) const
{
    return d_ptr->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, int keyPoints, ElaIconType awesome) const
{
    return d_ptr->_navigationBar->addPageNode(pageTitle, page, keyPoints, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints, ElaIconType awesome) const
{
    return d_ptr->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, keyPoints, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, ElaIconType awesome) const
{
    return d_ptr->_navigationBar->addFooterNode(footerTitle, nullptr, footerKey, keyPoints, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints, ElaIconType awesome) const
{
    return d_ptr->_navigationBar->addFooterNode(footerTitle, page, footerKey, keyPoints, awesome);
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
    if (getNavigationBarDisplayMode() == ElaNavigationType::Auto)
    {
        int contentMargin = d->_contentsMargins;
        int appBarHeight = d->_appBar->height();
        d->_adjustNavigationSize();
        if (width() >= 850 && d->_currentNavigationBarDisplayMode != ElaNavigationType::Maximal)
        {
            d->_resetWindowLayout(true);
            //导航栏动画
            ElaNavigationBarPrivate* navigationBarPrivate = d->_navigationBar->d_func();
            QPropertyAnimation* navigationMoveAnimation = new QPropertyAnimation(d->_navigationBar, "pos");
            connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [=]() {
                d->_resetWindowLayout(false);
                d->_navigationBar->switchCompact(false);
                d->_isNavagationAnimationFinished = true; });
            navigationMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
            navigationMoveAnimation->setDuration(300);

            //过渡动画
            d->_navigationBar->setFixedWidth(300);
            d->_navigationBar->move(QPoint(d->_centerStackedWidget->pos().x() - d->_navigationBar->width(), d->_navigationBar->pos().y()));
            QPropertyAnimation* maximalAnimation = new QPropertyAnimation(navigationBarPrivate->_compactWidget, "pos");
            while (navigationBarPrivate->_mainLayout->count() > 0)
            {
                navigationBarPrivate->_mainLayout->takeAt(0);
            }
            navigationBarPrivate->_maximalWidget->setVisible(true);
            connect(maximalAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                navigationBarPrivate->_maximalWidget->move(value.toPoint().x() - 300, value.toPoint().y());
            });
            maximalAnimation->setEasingCurve(QEasingCurve::OutSine);
            maximalAnimation->setDuration(300);
            maximalAnimation->setStartValue(QPoint(d->_navigationBar->width() - 45, navigationBarPrivate->_compactWidget->pos().y()));
            maximalAnimation->setEndValue(QPoint(d->_navigationBar->width(), navigationBarPrivate->_compactWidget->pos().y()));
            maximalAnimation->start(QAbstractAnimation::DeleteWhenStopped);

            navigationMoveAnimation->setStartValue(d->_navigationBar->pos());
            navigationMoveAnimation->setEndValue(QPoint(contentMargin, appBarHeight + contentMargin));

            //堆栈动画
            QPropertyAnimation* stackedMoveAnimation = new QPropertyAnimation(d->_centerStackedWidget, "pos");
            connect(stackedMoveAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { d->_centerStackedWidget->resize(width() - d->_centerStackedWidget->x() - 5, height() - 40); });
            stackedMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
            stackedMoveAnimation->setDuration(300);
            stackedMoveAnimation->setStartValue(d->_centerStackedWidget->pos());
            // 305 = 300  + 5
            stackedMoveAnimation->setEndValue(QPoint(305, appBarHeight + contentMargin));

            navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            stackedMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            d->_currentNavigationBarDisplayMode = ElaNavigationType::Maximal;
            d->_isNavigationBarExpanded = false;
            d->_appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint, false);
            d->_isNavagationAnimationFinished = false;
        }
        else if (width() >= 550 && width() < 850 && d->_currentNavigationBarDisplayMode != ElaNavigationType::Compact)
        {
            ElaNavigationBarPrivate* navigationBarPrivate = d->_navigationBar->d_func();
            ElaNavigationType::NavigationDisplayMode lastMode = d->_currentNavigationBarDisplayMode;
            QPropertyAnimation* navigationMoveAnimation = new QPropertyAnimation(d->_navigationBar, "pos");
            connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [=]() {
                d->_navigationBar->switchCompact(true);
                d->_resetWindowLayout(false);
                d->_isNavagationAnimationFinished = true;
            });
            //过渡动画
            if (lastMode == ElaNavigationType::Maximal)
            {
                QPropertyAnimation* maximalAnimation = new QPropertyAnimation(navigationBarPrivate->_compactWidget, "pos");
                while (navigationBarPrivate->_mainLayout->count() > 0)
                {
                    navigationBarPrivate->_mainLayout->takeAt(0);
                }
                navigationBarPrivate->_compactWidget->resize(40, navigationBarPrivate->q_func()->height());
                navigationBarPrivate->_compactWidget->setVisible(true);
                connect(maximalAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                    navigationBarPrivate->_maximalWidget->move(value.toPoint().x() - 300, value.toPoint().y());
                });
                maximalAnimation->setEasingCurve(QEasingCurve::OutSine);
                maximalAnimation->setDuration(300);
                maximalAnimation->setStartValue(QPoint(d->_navigationBar->width(), navigationBarPrivate->_compactWidget->pos().y()));
                maximalAnimation->setEndValue(QPoint(d->_navigationBar->width() - 40, navigationBarPrivate->_compactWidget->pos().y()));
                maximalAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            }

            if (d->_currentNavigationBarDisplayMode == ElaNavigationType::Maximal)
            {
                d->_navigationBar->show();
                navigationMoveAnimation->setEasingCurve(QEasingCurve::OutSine);
                navigationMoveAnimation->setDuration(300);
                navigationMoveAnimation->setStartValue(d->_navigationBar->pos());
                navigationMoveAnimation->setEndValue(QPoint(-d->_navigationBar->width() + d->_compactBarWidth, 35));
            }
            else
            {
                d->_navigationBar->switchCompact(true);
                d->_navigationBar->show();
                navigationMoveAnimation->setEasingCurve(QEasingCurve::OutSine);
                navigationMoveAnimation->setDuration(300);
                navigationMoveAnimation->setStartValue(d->_navigationBar->pos());
                navigationMoveAnimation->setEndValue(QPoint(d->_navigationBar->pos().x() + 50, d->_navigationBar->pos().y()));
            }

            //堆栈窗口移动
            QPropertyAnimation* stackedMoveAnimation = new QPropertyAnimation(d->_centerStackedWidget, "pos");
            connect(stackedMoveAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { d->_centerStackedWidget->resize(width() - d->_centerStackedWidget->pos().x() - 5, height() - 40); });
            stackedMoveAnimation->setDuration(300);
            stackedMoveAnimation->setEasingCurve(QEasingCurve::OutSine);
            stackedMoveAnimation->setStartValue(d->_centerStackedWidget->pos());
            stackedMoveAnimation->setEndValue(QPoint(contentMargin + d->_compactBarWidth, appBarHeight + contentMargin));
            d->_resetWindowLayout(true);
            navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            stackedMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            d->_currentNavigationBarDisplayMode = ElaNavigationType::Compact;
            d->_isNavigationBarExpanded = false;
            d->_appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint, false);
            d->_isNavagationAnimationFinished = false;
        }
        else if (width() < 550 && d->_currentNavigationBarDisplayMode != ElaNavigationType::Minimal)
        {
            QPropertyAnimation* navigationMoveAnimation = new QPropertyAnimation(d->_navigationBar, "pos");
            connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [=]() {
                d->_navigationBar->hide();
                d->_navigationBar->switchCompact(true);
                d->_centerLayout->addWidget(d->_centerStackedWidget);
                d->_mainLayout->takeAt(2);
                QMap<QString, QVariant> postData;
                postData.insert("NavigationAnimationState", false);
                ElaEventBus::getInstance()->post("NavigationAnimationStateChanged", postData);
                d->_isNavagationAnimationFinished = true; });
            navigationMoveAnimation->setEasingCurve(QEasingCurve::OutSine);
            navigationMoveAnimation->setDuration(300);
            navigationMoveAnimation->setStartValue(d->_navigationBar->pos());
            navigationMoveAnimation->setEndValue(QPoint(-d->_navigationBar->width(), 35));

            QPropertyAnimation* stackedMoveAnimation = new QPropertyAnimation(d->_centerStackedWidget, "pos");
            connect(stackedMoveAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { d->_centerStackedWidget->resize(width() - d->_centerStackedWidget->pos().x() - 5, height() - 40); });
            stackedMoveAnimation->setDuration(300);
            stackedMoveAnimation->setEasingCurve(QEasingCurve::OutSine);
            stackedMoveAnimation->setStartValue(d->_centerStackedWidget->pos());
            stackedMoveAnimation->setEndValue(QPoint(contentMargin, appBarHeight + contentMargin));
            d->_resetWindowLayout(true);
            navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            stackedMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            d->_currentNavigationBarDisplayMode = ElaNavigationType::Minimal;
            d->_isNavigationBarExpanded = false;
            d->_appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint);
            d->_isNavagationAnimationFinished = false;
        }
    }
    QWidget::resizeEvent(event);
}
