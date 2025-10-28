#include "ElaWindow.h"
#include "ElaApplication.h"
#include "ElaCentralStackedWidget.h"
#include "ElaEventBus.h"
#include "ElaMenu.h"
#include "ElaNavigationBar.h"
#include "ElaNavigationRouter.h"
#include "ElaTheme.h"
#include "ElaWindowStyle.h"
#include "private/ElaAppBarPrivate.h"
#include "private/ElaNavigationBarPrivate.h"
#include "private/ElaWindowPrivate.h"
#include <QApplication>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QMovie>
#include <QPainter>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScreen>
#include <QStyleOption>
#include <QToolBar>
#include <QtMath>
Q_PROPERTY_CREATE_Q_CPP(ElaWindow, int, ThemeChangeTime)
Q_PROPERTY_CREATE_Q_CPP(ElaWindow, ElaNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
Q_PROPERTY_CREATE_Q_CPP(ElaWindow, ElaWindowType::StackSwitchMode, StackSwitchMode)
Q_TAKEOVER_NATIVEEVENT_CPP(ElaWindow, d_func()->_appBar);
ElaWindow::ElaWindow(QWidget* parent)
    : QMainWindow{parent}, d_ptr(new ElaWindowPrivate())
{
    Q_D(ElaWindow);
    d->q_ptr = this;
    d->_pStackSwitchMode = ElaWindowType::StackSwitchMode::Popup;
    setProperty("ElaBaseClassName", "ElaWindow");
    resize(1020, 680); // 默认宽高

    d->_pThemeChangeTime = 700;
    d->_pNavigationBarDisplayMode = ElaNavigationType::NavigationDisplayMode::Auto;
    connect(this, &ElaWindow::pNavigationBarDisplayModeChanged, d, &ElaWindowPrivate::onDisplayModeChanged);

    // 自定义AppBar
    d->_appBar = new ElaAppBar(this);
    connect(d->_appBar, &ElaAppBar::routeBackButtonClicked, this, []() {
        ElaNavigationRouter::getInstance()->navigationRouteBack();
    });
    connect(d->_appBar, &ElaAppBar::routeForwardButtonClicked, this, []() {
        ElaNavigationRouter::getInstance()->navigationRouteForward();
    });
    connect(d->_appBar, &ElaAppBar::closeButtonClicked, this, &ElaWindow::closeButtonClicked);
    // 导航栏
    d->_navigationBar = new ElaNavigationBar(this);
    // 返回按钮状态变更
    connect(ElaNavigationRouter::getInstance(), &ElaNavigationRouter::navigationRouterStateChanged, d, &ElaWindowPrivate::onNavigationRouterStateChanged);

    // 转发用户卡片点击信号
    connect(d->_navigationBar, &ElaNavigationBar::userInfoCardClicked, this, &ElaWindow::userInfoCardClicked);
    // 转发点击信号
    connect(d->_navigationBar, &ElaNavigationBar::navigationNodeClicked, this, &ElaWindow::navigationNodeClicked);
    // 跳转处理
    connect(d->_navigationBar, &ElaNavigationBar::navigationNodeClicked, d, &ElaWindowPrivate::onNavigationNodeClicked);
    // 新增窗口
    connect(d->_navigationBar, &ElaNavigationBar::navigationNodeAdded, d, &ElaWindowPrivate::onNavigationNodeAdded);
    // 移除窗口
    connect(d->_navigationBar, &ElaNavigationBar::navigationNodeRemoved, d, &ElaWindowPrivate::onNavigationNodeRemoved);
    // 在新窗口打开
    connect(d->_navigationBar, &ElaNavigationBar::pageOpenInNewWindow, this, &ElaWindow::pageOpenInNewWindow);

    // 导航中心堆栈窗口
    d->_navigationCenterStackedWidget = new ElaCentralStackedWidget(this);
    d->_navigationCenterStackedWidget->setContentsMargins(0, 0, 0, 0);
    QWidget* navigationCentralWidget = new QWidget(this);
    navigationCentralWidget->setObjectName("ElaWindowNavigationCentralWidget");
    navigationCentralWidget->setStyleSheet("#ElaWindowNavigationCentralWidget{background-color:transparent;}");
    navigationCentralWidget->installEventFilter(this);
    d->_centerLayout = new QHBoxLayout(navigationCentralWidget);
    d->_centerLayout->setSpacing(0);
    d->_centerLayout->addWidget(d->_navigationBar);
    d->_centerLayout->addWidget(d->_navigationCenterStackedWidget);
    d->_centerLayout->setContentsMargins(d->_contentsMargins, 0, 0, 0);

    // 事件总线
    d->_focusEvent = new ElaEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();

    // 展开导航栏
    connect(d->_appBar, &ElaAppBar::navigationButtonClicked, d, &ElaWindowPrivate::onNavigationButtonClicked);

    // 主题变更动画
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaWindowPrivate::onThemeModeChanged);
    connect(d->_appBar, &ElaAppBar::themeChangeButtonClicked, d, &ElaWindowPrivate::onThemeReadyChange);
    d->_isInitFinished = true;

    // 中心堆栈窗口
    d->_centerStackedWidget = new ElaCentralStackedWidget(this);
    d->_centerStackedWidget->setIsTransparent(true);
    d->_centerStackedWidget->getContainerStackedWidget()->addWidget(navigationCentralWidget);
    setCentralWidget(d->_centerStackedWidget);
    setObjectName("ElaWindow");
    setStyleSheet("#ElaWindow{background-color:transparent;}");
    setStyle(new ElaWindowStyle(style()));

    eApp->syncWindowDisplayMode(this);
    d->_windowDisplayMode = eApp->getWindowDisplayMode();
    connect(eApp, &ElaApplication::pWindowDisplayModeChanged, d, &ElaWindowPrivate::onWindowDisplayModeChanged);

    d->_pWindowPaintMode = ElaWindowType::PaintMode::Normal;
    d->_lightWindowPix = new QPixmap();
    d->_darkWindowPix = new QPixmap();

    d->_windowPaintMovie = new QMovie(this);
    connect(d->_windowPaintMovie, &QMovie::frameChanged, this, [=]() {
        update();
    });
}

ElaWindow::~ElaWindow()
{
    Q_D(ElaWindow);
    eApp->syncWindowDisplayMode(this, false);
    delete this->style();
    delete d->_lightWindowPix;
    delete d->_darkWindowPix;
}

void ElaWindow::setIsStayTop(bool isStayTop)
{
    Q_D(ElaWindow);
    d->_appBar->setIsStayTop(isStayTop);
    Q_EMIT pIsStayTopChanged();
}

bool ElaWindow::getIsStayTop() const
{
    return d_ptr->_appBar->getIsStayTop();
}

void ElaWindow::setIsFixedSize(bool isFixedSize)
{
    Q_D(ElaWindow);
    d->_appBar->setIsFixedSize(isFixedSize);
    Q_EMIT pIsFixedSizeChanged();
}

bool ElaWindow::getIsFixedSize() const
{
    return d_ptr->_appBar->getIsFixedSize();
}

void ElaWindow::setIsDefaultClosed(bool isDefaultClosed)
{
    Q_D(ElaWindow);
    d->_appBar->setIsDefaultClosed(isDefaultClosed);
    Q_EMIT pIsDefaultClosedChanged();
}

bool ElaWindow::getIsDefaultClosed() const
{
    return d_ptr->_appBar->getIsDefaultClosed();
}

void ElaWindow::setAppBarHeight(int appBarHeight)
{
    Q_D(ElaWindow);
    d->_appBar->setAppBarHeight(appBarHeight);
    Q_EMIT pAppBarHeightChanged();
}

int ElaWindow::getAppBarHeight() const
{
    Q_D(const ElaWindow);
    return d->_appBar->getAppBarHeight();
}

void ElaWindow::setCustomWidget(ElaAppBarType::CustomArea customArea, QWidget* widget, QObject* hitTestObject, const QString& hitTestFunctionName)
{
    Q_D(ElaWindow);
    d->_appBar->setCustomWidget(customArea, widget, hitTestObject, hitTestFunctionName);
    Q_EMIT customWidgetChanged();
}

QWidget* ElaWindow::getCustomWidget(ElaAppBarType::CustomArea customArea) const
{
    Q_D(const ElaWindow);
    return d->_appBar->getCustomWidget(customArea);
}

void ElaWindow::setCentralCustomWidget(QWidget* customWidget)
{
    Q_D(ElaWindow);
    d->_navigationCenterStackedWidget->setCustomWidget(customWidget);
    Q_EMIT centralCustomWidgetChanged();
}

QWidget* ElaWindow::getCentralCustomWidget() const
{
    Q_D(const ElaWindow);
    return d->_navigationCenterStackedWidget->getCustomWidget();
}

void ElaWindow::setCustomMenu(QMenu* customMenu)
{
    Q_D(ElaWindow);
    d->_appBar->setCustomMenu(customMenu);
    Q_EMIT customMenuChanged();
}

QMenu* ElaWindow::getCustomMenu() const
{
    Q_D(const ElaWindow);
    return d->_appBar->getCustomMenu();
}

void ElaWindow::setIsCentralStackedWidgetTransparent(bool isTransparent)
{
    Q_D(ElaWindow);
    d->_navigationCenterStackedWidget->setIsTransparent(isTransparent);
}

bool ElaWindow::getIsCentralStackedWidgetTransparent() const
{
    Q_D(const ElaWindow);
    return d->_navigationCenterStackedWidget->getIsTransparent();
}

void ElaWindow::setIsAllowPageOpenInNewWindow(bool isAllowPageOpenInNewWindow)
{
    Q_D(ElaWindow);
    d->_navigationBar->setIsAllowPageOpenInNewWindow(isAllowPageOpenInNewWindow);
    Q_EMIT pIsAllowPageOpenInNewWindowChanged();
}

bool ElaWindow::getIsAllowPageOpenInNewWindow() const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->getIsAllowPageOpenInNewWindow();
}

void ElaWindow::setNavigationBarWidth(int navigationBarWidth)
{
    Q_D(ElaWindow);
    d->_navigationBar->setNavigationBarWidth(navigationBarWidth);
    Q_EMIT pNavigationBarWidthChanged();
}

int ElaWindow::getNavigationBarWidth() const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->getNavigationBarWidth();
}

void ElaWindow::setCurrentStackIndex(int currentStackIndex)
{
    Q_D(ElaWindow);
    if (currentStackIndex >= d->_centerStackedWidget->getContainerStackedWidget()->count() || currentStackIndex < 0 || currentStackIndex == d->_centralStackTargetIndex)
    {
        return;
    }
    d->_centralStackTargetIndex = currentStackIndex;
    QVariantMap routeData;
    int currentCenterStackedWidgetIndex = d->_centerStackedWidget->getContainerStackedWidget()->currentIndex();
    routeData.insert("ElaBackCentralStackIndex", currentCenterStackedWidgetIndex);
    routeData.insert("ElaForwardCentralStackIndex", currentStackIndex);
    ElaNavigationRouter::getInstance()->navigationRoute(d, "onNavigationRoute", routeData);
    d->_centerStackedWidget->doWindowStackSwitch(d->_pStackSwitchMode, currentStackIndex, false);
    Q_EMIT pCurrentStackIndexChanged();
}

int ElaWindow::getCurrentStackIndex() const
{
    Q_D(const ElaWindow);
    return d->_centerStackedWidget->getContainerStackedWidget()->currentIndex();
}

void ElaWindow::setWindowPaintMode(ElaWindowType::PaintMode windowPaintMode)
{
    Q_D(ElaWindow);
    if (d->_windowPaintMovie->state() == QMovie::Running)
    {
        d->_windowPaintMovie->stop();
    }
    if (windowPaintMode == ElaWindowType::PaintMode::Movie)
    {
        d->_windowPaintMovie->setFileName(d->_themeMode == ElaThemeType::Light ? d->_lightWindowMoviePath : d->_darkWindowMoviePath);
        d->_windowPaintMovie->start();
    }
    d->_pWindowPaintMode = windowPaintMode;
    update();
    Q_EMIT pWindowPaintModeChanged();
}

ElaWindowType::PaintMode ElaWindow::getWindowPaintMode() const
{
    Q_D(const ElaWindow);
    return d->_pWindowPaintMode;
}

void ElaWindow::moveToCenter()
{
    if (isMaximized() || isFullScreen())
    {
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto geometry = screen()->availableGeometry();
#else
    auto geometry = qApp->screenAt(this->geometry().center())->geometry();
#endif
    setGeometry((geometry.left() + geometry.right() - width()) / 2, (geometry.top() + geometry.bottom() - height()) / 2, width(), height());
}

void ElaWindow::setIsNavigationBarEnable(bool isVisible)
{
    Q_D(ElaWindow);
    d->_isNavigationEnable = isVisible;
    d->_navigationBar->setVisible(isVisible);
    d->_centerLayout->setContentsMargins(isVisible ? d->_contentsMargins : 0, 0, 0, 0);
    d->_navigationCenterStackedWidget->setIsHasRadius(isVisible);
}

bool ElaWindow::getIsNavigationBarEnable() const
{
    return d_ptr->_isNavigationEnable;
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

ElaNavigationType::NodeOperateReturnType ElaWindow::addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType::IconName awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addExpanderNode(expanderTitle, expanderKey, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType::IconName awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, ElaIconType::IconName awesome)
{
    Q_D(ElaWindow);
    auto returnType = d->_navigationBar->addPageNode(pageTitle, page, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        d->_pageMetaMap.insert(page->property("ElaPageKey").toString(), page->metaObject());
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, ElaIconType::IconName awesome)
{
    Q_D(ElaWindow);
    auto returnType = d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        d->_pageMetaMap.insert(page->property("ElaPageKey").toString(), page->metaObject());
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, int keyPoints, ElaIconType::IconName awesome)
{
    Q_D(ElaWindow);
    auto returnType = d->_navigationBar->addPageNode(pageTitle, page, keyPoints, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        d->_pageMetaMap.insert(page->property("ElaPageKey").toString(), page->metaObject());
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints, ElaIconType::IconName awesome)
{
    Q_D(ElaWindow);
    auto returnType = d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, keyPoints, awesome);
    if (returnType == ElaNavigationType::Success)
    {
        d->_pageMetaMap.insert(page->property("ElaPageKey").toString(), page->metaObject());
    }
    return returnType;
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, ElaIconType::IconName awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addFooterNode(footerTitle, nullptr, footerKey, keyPoints, awesome);
}

ElaNavigationType::NodeOperateReturnType ElaWindow::addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints, ElaIconType::IconName awesome)
{
    Q_D(ElaWindow);
    auto returnType = d->_navigationBar->addFooterNode(footerTitle, page, footerKey, keyPoints, awesome);
    if (page && returnType == ElaNavigationType::Success)
    {
        d->_pageMetaMap.insert(page->property("ElaPageKey").toString(), page->metaObject());
    }
    return returnType;
}

void ElaWindow::addCentralWidget(QWidget* centralWidget)
{
    Q_D(ElaWindow);
    if (!centralWidget)
    {
        return;
    }
    d->_centerStackedWidget->getContainerStackedWidget()->addWidget(centralWidget);
}

QWidget* ElaWindow::getCentralWidget(int index) const
{
    Q_D(const ElaWindow);
    if (index >= d->_centerStackedWidget->getContainerStackedWidget()->count() || index < 1)
    {
        return nullptr;
    }
    return d->_centerStackedWidget->getContainerStackedWidget()->widget(index);
}

bool ElaWindow::getNavigationNodeIsExpanded(QString expanderKey) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->getNavigationNodeIsExpanded(expanderKey);
}

void ElaWindow::expandNavigationNode(QString expanderKey)
{
    Q_D(ElaWindow);
    d->_navigationBar->expandNavigationNode(expanderKey);
}

void ElaWindow::collapseNavigationNode(QString expanderKey)
{
    Q_D(ElaWindow);
    d->_navigationBar->collapseNavigationNode(expanderKey);
}

void ElaWindow::removeNavigationNode(QString nodeKey) const
{
    Q_D(const ElaWindow);
    d->_navigationBar->removeNavigationNode(nodeKey);
}

int ElaWindow::getPageOpenInNewWindowCount(QString nodeKey) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->getPageOpenInNewWindowCount(nodeKey);
}

void ElaWindow::backtrackNavigationNode(QString nodeKey)
{
    Q_D(ElaWindow);
    const QMetaObject* meta = d->_pageMetaMap.value(nodeKey);
    if (!meta)
    {
        return;
    }
    QWidget* widget = dynamic_cast<QWidget*>(meta->newInstance());
    if (widget)
    {
        auto originWidget = d->_routeMap[nodeKey];
        int currentIndex = d->_navigationCenterStackedWidget->getContainerStackedWidget()->currentIndex();
        int originIndex = d->_navigationCenterStackedWidget->getContainerStackedWidget()->indexOf(originWidget);
        widget->setProperty("ElaPageKey", nodeKey);
        d->_routeMap[nodeKey] = widget;
        d->_navigationCenterStackedWidget->getContainerStackedWidget()->insertWidget(originIndex, widget);
        d->_navigationCenterStackedWidget->getContainerStackedWidget()->setCurrentIndex(currentIndex);
        d->_navigationCenterStackedWidget->getContainerStackedWidget()->removeWidget(originWidget);
        originWidget->deleteLater();
    }
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

void ElaWindow::setNavigationNodeTitle(QString nodeKey, QString nodeTitle)
{
    Q_D(ElaWindow);
    d->_navigationBar->setNavigationNodeTitle(nodeKey, nodeTitle);
}

QString ElaWindow::getNavigationNodeTitle(QString nodeKey) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->getNavigationNodeTitle(nodeKey);
}

void ElaWindow::navigation(QString pageKey)
{
    Q_D(ElaWindow);
    d->_navigationBar->navigation(pageKey);
}

int ElaWindow::getCurrentNavigationIndex() const
{
    Q_D(const ElaWindow);
    return d->_navigationCenterStackedWidget->getContainerStackedWidget()->currentIndex();
}

QString ElaWindow::getCurrentNavigationPageKey() const
{
    Q_D(const ElaWindow);
    return d->_navigationCenterStackedWidget->getContainerStackedWidget()->currentWidget()->property("ElaPageKey").toString();
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

void ElaWindow::setWindowMoviePath(ElaThemeType::ThemeMode themeMode, const QString& moviePath)
{
    Q_D(ElaWindow);
    if (themeMode == ElaThemeType::ThemeMode::Light)
    {
        d->_lightWindowMoviePath = moviePath;
    }
    else
    {
        d->_darkWindowMoviePath = moviePath;
    }
    if (d->_themeMode == themeMode && d->_pWindowPaintMode == ElaWindowType::PaintMode::Movie)
    {
        if (d->_windowPaintMovie->state() == QMovie::Running)
        {
            d->_windowPaintMovie->stop();
        }
        d->_windowPaintMovie->setFileName(moviePath);
        d->_windowPaintMovie->start();
    }
}

QString ElaWindow::getWindowMoviePath(ElaThemeType::ThemeMode themeMode) const
{
    Q_D(const ElaWindow);
    return themeMode == ElaThemeType::Light ? d->_lightWindowMoviePath : d->_darkWindowMoviePath;
}

void ElaWindow::setWindowMovieRate(qreal rate)
{
    Q_D(ElaWindow);
    d->_windowPaintMovie->setSpeed(rate * 100);
}

qreal ElaWindow::getWindowMovieRate() const
{
    Q_D(const ElaWindow);
    return d->_windowPaintMovie->speed() / 100.0;
}

void ElaWindow::setWindowPixmap(ElaThemeType::ThemeMode themeMode, const QPixmap& pixmap)
{
    Q_D(ElaWindow);
    if (themeMode == ElaThemeType::ThemeMode::Light)
    {
        *d->_lightWindowPix = pixmap;
    }
    else
    {
        *d->_darkWindowPix = pixmap;
    }
    update();
}

QPixmap ElaWindow::getWindowPixmap(ElaThemeType::ThemeMode themeMode) const
{
    Q_D(const ElaWindow);
    return themeMode == ElaThemeType::Light ? *d->_lightWindowPix : *d->_darkWindowPix;
}

void ElaWindow::closeWindow()
{
    Q_D(ElaWindow);
    d->_isWindowClosing = true;
    d->_appBar->closeWindow();
}

bool ElaWindow::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(ElaWindow);
    switch (event->type())
    {
    case QEvent::Resize:
    {
        d->_doNavigationDisplayModeChange();
        break;
    }
    default:
    {
        break;
    }
    }
    return QMainWindow::eventFilter(watched, event);
}

QMenu* ElaWindow::createPopupMenu()
{
    ElaMenu* menu = nullptr;
    QList<QDockWidget*> dockwidgets = findChildren<QDockWidget*>();
    if (dockwidgets.size())
    {
        menu = new ElaMenu(this);
        for (int i = 0; i < dockwidgets.size(); ++i)
        {
            QDockWidget* dockWidget = dockwidgets.at(i);
            if (dockWidget->parentWidget() == this)
            {
                menu->addAction(dockwidgets.at(i)->toggleViewAction());
            }
        }
        menu->addSeparator();
    }

    QList<QToolBar*> toolbars = findChildren<QToolBar*>();
    if (toolbars.size())
    {
        if (!menu)
        {
            menu = new ElaMenu(this);
        }
        for (int i = 0; i < toolbars.size(); ++i)
        {
            QToolBar* toolBar = toolbars.at(i);
            if (toolBar->parentWidget() == this)
            {
                menu->addAction(toolbars.at(i)->toggleViewAction());
            }
        }
    }
    if (menu)
    {
        menu->setMenuItemHeight(28);
    }
    return menu;
}

void ElaWindow::paintEvent(QPaintEvent* event)
{
    Q_D(ElaWindow);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    switch (d->_windowDisplayMode)
    {
    case ElaApplicationType::Normal:
    {
        switch (d->_pWindowPaintMode)
        {
        case ElaWindowType::Normal:
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(ElaThemeColor(d->_themeMode, WindowBase));
            painter.drawRect(rect());
            break;
        }
        case ElaWindowType::Pixmap:
        {
            QPixmap* pix = d->_themeMode == ElaThemeType::Light ? d->_lightWindowPix : d->_darkWindowPix;
            qreal windowAspectRatio = (qreal)rect().width() / rect().height();
            qreal pixAspectRatio = (qreal)pix->width() / pix->height();
            int targetPixWidth, targetPixHeight;
            if (windowAspectRatio < pixAspectRatio)
            {
                targetPixWidth = qRound(pix->width() * windowAspectRatio / pixAspectRatio);
                targetPixHeight = pix->height();
            }
            else
            {
                targetPixWidth = pix->width();
                targetPixHeight = qRound(pix->height() * pixAspectRatio / windowAspectRatio);
            }
            painter.drawPixmap(rect(), *pix, QRect((pix->width() - targetPixWidth) / 2, (pix->height() - targetPixHeight) / 2, targetPixWidth, targetPixHeight));
            break;
        }
        case ElaWindowType::Movie:
        {
            QPixmap pix = d->_windowPaintMovie->currentPixmap();
            qreal windowAspectRatio = (qreal)rect().width() / rect().height();
            qreal pixAspectRatio = (qreal)pix.width() / pix.height();
            int targetPixWidth, targetPixHeight;
            if (windowAspectRatio < pixAspectRatio)
            {
                targetPixWidth = qRound(pix.width() * windowAspectRatio / pixAspectRatio);
                targetPixHeight = pix.height();
            }
            else
            {
                targetPixWidth = pix.width();
                targetPixHeight = qRound(pix.height() * pixAspectRatio / windowAspectRatio);
            }
            painter.drawPixmap(rect(), pix, QRect((pix.width() - targetPixWidth) / 2, (pix.height() - targetPixHeight) / 2, targetPixWidth, targetPixHeight));
            break;
        }
        default:
        {
            break;
        }
        }
    }
    default:
    {
        break;
    }
    }
    painter.restore();
}
