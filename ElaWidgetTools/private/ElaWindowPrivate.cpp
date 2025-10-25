#include "ElaWindowPrivate.h"

#include "ElaAppBarPrivate.h"
#include "ElaApplication.h"
#include "ElaCentralStackedWidget.h"
#include "ElaNavigationBar.h"
#include "ElaTheme.h"
#include "ElaThemeAnimationWidget.h"
#include "ElaWindow.h"
#include <QApplication>
#include <QPropertyAnimation>
#include <QTimer>
#include <QVBoxLayout>
#include <QtMath>
ElaWindowPrivate::ElaWindowPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaWindowPrivate::~ElaWindowPrivate()
{
}

void ElaWindowPrivate::onNavigationButtonClicked()
{
    if (_isWMClickedAnimationFinished)
    {
        _isNavigationDisplayModeChanged = false;
        _resetWindowLayout(true);
        _navigationBar->setIsTransparent(false);
        _navigationBar->setDisplayMode(ElaNavigationType::Maximal, false);
        _navigationBar->move(-_navigationBar->width(), _navigationBar->pos().y());
        _navigationBar->resize(_navigationBar->width(), _navigationCenterStackedWidget->height() + 1);
        QPropertyAnimation* navigationMoveAnimation = new QPropertyAnimation(_navigationBar, "pos");
        connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [=]() {
            _isNavigationBarExpanded = true;
        });
        navigationMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
        navigationMoveAnimation->setDuration(300);
        navigationMoveAnimation->setStartValue(_navigationBar->pos());
        navigationMoveAnimation->setEndValue(QPoint(0, 0));
        navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        _isWMClickedAnimationFinished = false;
    }
}

void ElaWindowPrivate::onWMWindowClickedEvent(QVariantMap data)
{
    ElaAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<ElaAppBarType::WMMouseActionType>();
    if (actionType == ElaAppBarType::WMLBUTTONDBLCLK || actionType == ElaAppBarType::WMLBUTTONUP || actionType == ElaAppBarType::WMNCLBUTTONDOWN)
    {
        if (ElaApplication::containsCursorToItem(_navigationBar))
        {
            return;
        }
        if (_isNavigationBarExpanded)
        {
            QPropertyAnimation* navigationMoveAnimation = new QPropertyAnimation(_navigationBar, "pos");
            connect(navigationMoveAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                if (_isNavigationDisplayModeChanged)
                {
                    _isWMClickedAnimationFinished = true;
                    _resetWindowLayout(false);
                    navigationMoveAnimation->deleteLater();
                }
            });
            connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [=]() {
                if (!_isNavigationDisplayModeChanged)
                {
                    _navigationBar->setDisplayMode(ElaNavigationType::Minimal, false);
                    _resetWindowLayout(false);
                }
                _isWMClickedAnimationFinished = true;
            });
            navigationMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
            navigationMoveAnimation->setDuration(300);
            navigationMoveAnimation->setStartValue(_navigationBar->pos());
            navigationMoveAnimation->setEndValue(QPoint(-_navigationBar->width(), 0));
            navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            _isNavigationBarExpanded = false;
        }
    }
}

void ElaWindowPrivate::onThemeReadyChange()
{
    Q_Q(ElaWindow);
    // 主题变更绘制窗口
    switch (eApp->getWindowDisplayMode())
    {
    case ElaApplicationType::Normal:
    case ElaApplicationType::ElaMica:
    {
        _appBar->setIsOnlyAllowMinAndClose(true);
        if (!_animationWidget)
        {
            QPoint centerPos = q->mapFromGlobal(QCursor::pos());
            _animationWidget = new ElaThemeAnimationWidget(q);
            connect(_animationWidget, &ElaThemeAnimationWidget::animationFinished, this, [=]() {
                _appBar->setIsOnlyAllowMinAndClose(false);
                _animationWidget = nullptr;
            });
            _animationWidget->move(0, 0);
            _animationWidget->setOldWindowBackground(q->grab(q->rect()).toImage());
            if (eTheme->getThemeMode() == ElaThemeType::Light)
            {
                eTheme->setThemeMode(ElaThemeType::Dark);
            }
            else
            {
                eTheme->setThemeMode(ElaThemeType::Light);
            }

            if (_pWindowPaintMode == ElaWindowType::PaintMode::Movie)
            {
                if (_windowPaintMovie->state() == QMovie::Running)
                {
                    _windowPaintMovie->stop();
                }
                _windowPaintMovie->setFileName(_themeMode == ElaThemeType::Light ? _lightWindowMoviePath : _darkWindowMoviePath);
                _windowPaintMovie->start();
            }
            _animationWidget->setCenter(centerPos);
            qreal topLeftDis = _distance(centerPos, QPoint(0, 0));
            qreal topRightDis = _distance(centerPos, QPoint(q->width(), 0));
            qreal bottomLeftDis = _distance(centerPos, QPoint(0, q->height()));
            qreal bottomRightDis = _distance(centerPos, QPoint(q->width(), q->height()));
            QList<qreal> disList{topLeftDis, topRightDis, bottomLeftDis, bottomRightDis};
            std::sort(disList.begin(), disList.end());
            _animationWidget->setEndRadius(disList[3]);
            _animationWidget->resize(q->width(), q->height());
            _animationWidget->startAnimation(_pThemeChangeTime);
            _animationWidget->show();
        }
        break;
    }
    default:
    {
        if (eTheme->getThemeMode() == ElaThemeType::Light)
        {
            eTheme->setThemeMode(ElaThemeType::Dark);
        }
        else
        {
            eTheme->setThemeMode(ElaThemeType::Light);
        }
        break;
    }
    }
}

void ElaWindowPrivate::onDisplayModeChanged()
{
    _currentNavigationBarDisplayMode = _pNavigationBarDisplayMode;
    switch (_pNavigationBarDisplayMode)
    {
    case ElaNavigationType::Auto:
    {
        _appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint, false);
        _doNavigationDisplayModeChange();
        break;
    }
    case ElaNavigationType::Minimal:
    {
        _navigationBar->setDisplayMode(ElaNavigationType::Minimal, true);
        _appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint);
        break;
    }
    case ElaNavigationType::Compact:
    {
        _navigationBar->setDisplayMode(ElaNavigationType::Compact, true);
        _appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint, false);
        break;
    }
    case ElaNavigationType::Maximal:
    {
        _navigationBar->setDisplayMode(ElaNavigationType::Maximal, true);
        _appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint, false);
        break;
    }
    }
}

void ElaWindowPrivate::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaWindow);
    _themeMode = themeMode;
    q->update();
}

void ElaWindowPrivate::onWindowDisplayModeChanged()
{
    Q_Q(ElaWindow);
    _windowDisplayMode = eApp->getWindowDisplayMode();
    if (_windowPaintMovie->state() == QMovie::Running)
    {
        _windowPaintMovie->stop();
    }
    if (_windowDisplayMode == ElaApplicationType::WindowDisplayMode::Normal && _pWindowPaintMode == ElaWindowType::Movie)
    {
        _windowPaintMovie->setFileName(_themeMode == ElaThemeType::Light ? _lightWindowMoviePath : _darkWindowMoviePath);
        _windowPaintMovie->start();
    }
    q->update();
}

void ElaWindowPrivate::onNavigationNodeClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, bool isRouteBack)
{
    QWidget* page = _routeMap.value(nodeKey);
    if (!page)
    {
        // 页脚没有绑定页面
        return;
    }
    int nodeIndex = _navigationCenterStackedWidget->getContainerStackedWidget()->indexOf(page);
    if (_navigationTargetIndex == nodeIndex || _navigationCenterStackedWidget->getContainerStackedWidget()->count() <= nodeIndex)
    {
        return;
    }
    _navigationTargetIndex = nodeIndex;
    _navigationCenterStackedWidget->doWindowStackSwitch(_pStackSwitchMode, nodeIndex, isRouteBack);
}

void ElaWindowPrivate::onNavigationNodeAdded(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page)
{
    if (nodeType == ElaNavigationType::PageNode)
    {
        _routeMap.insert(nodeKey, page);
        _navigationCenterStackedWidget->getContainerStackedWidget()->addWidget(page);
    }
    else
    {
        _routeMap.insert(nodeKey, page);
        if (page)
        {
            _navigationCenterStackedWidget->getContainerStackedWidget()->addWidget(page);
        }
    }
}

void ElaWindowPrivate::onNavigationNodeRemoved(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey)
{
    Q_Q(ElaWindow);
    if (!_routeMap.contains(nodeKey))
    {
        return;
    }
    QWidget* page = _routeMap.value(nodeKey);
    _routeMap.remove(nodeKey);
    _pageMetaMap.remove(nodeKey);
    _navigationCenterStackedWidget->getContainerStackedWidget()->removeWidget(page);
    QWidget* currentWidget = _navigationCenterStackedWidget->getContainerStackedWidget()->currentWidget();
    if (currentWidget)
    {
        q->navigation(currentWidget->property("ElaPageKey").toString());
    }
}

void ElaWindowPrivate::onNavigationRouterStateChanged(ElaNavigationRouterType::RouteMode routeMode)
{
    switch (routeMode)
    {
    case ElaNavigationRouterType::BackValid:
    {
        _appBar->setRouteBackButtonEnable(true);
        break;
    }
    case ElaNavigationRouterType::BackInvalid:
    {
        _appBar->setRouteBackButtonEnable(false);
        break;
    }
    case ElaNavigationRouterType::ForwardValid:
    {
        _appBar->setRouteForwardButtonEnable(true);
        break;
    }
    case ElaNavigationRouterType::ForwardInvalid:
    {
        _appBar->setRouteForwardButtonEnable(false);
        break;
    }
    }
}

void ElaWindowPrivate::onNavigationRoute(QVariantMap routeData)
{
    Q_Q(ElaWindow);
    int routeIndex = -1;
    _centralStackTargetIndex = routeIndex;
    bool isRouteBack = routeData.value("ElaRouteBackMode").toBool();
    if (isRouteBack)
    {
        routeIndex = routeData.value("ElaBackCentralStackIndex").toUInt();
    }
    else
    {
        routeIndex = routeData.value("ElaForwardCentralStackIndex").toUInt();
    }
    if (routeIndex != _centerStackedWidget->getContainerStackedWidget()->currentIndex())
    {
        _centerStackedWidget->doWindowStackSwitch(_pStackSwitchMode, routeIndex, isRouteBack);
    }
}

qreal ElaWindowPrivate::_distance(QPoint point1, QPoint point2)
{
    return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) + (point1.y() - point2.y()) * (point1.y() - point2.y()));
}

void ElaWindowPrivate::_resetWindowLayout(bool isAnimation)
{
    if (isAnimation)
    {
        while (_centerLayout->count() > 0)
        {
            _centerLayout->takeAt(0);
        }
    }
    else
    {
        if (_centerLayout->count() == 0)
        {
            _navigationBar->setIsTransparent(true);
            _navigationBar->setDisplayMode(ElaNavigationType::Minimal, false);
            _centerLayout->addWidget(_navigationBar);
            _centerLayout->addWidget(_navigationCenterStackedWidget);
        }
    }
}

void ElaWindowPrivate::_doNavigationDisplayModeChange()
{
    Q_Q(ElaWindow);
    if (_isWindowClosing || !_isNavigationEnable || !_isInitFinished)
    {
        return;
    }
    if (_pNavigationBarDisplayMode == ElaNavigationType::Minimal)
    {
        _resetWindowLayout(false);
    }
    if (_pNavigationBarDisplayMode == ElaNavigationType::Auto)
    {
        _isNavigationDisplayModeChanged = true;
        _isWMClickedAnimationFinished = true;
        _resetWindowLayout(false);
        int width = q->centralWidget()->width();
        if (width >= 850 && _currentNavigationBarDisplayMode != ElaNavigationType::Maximal)
        {
            _navigationBar->setDisplayMode(ElaNavigationType::Maximal);
            _currentNavigationBarDisplayMode = ElaNavigationType::Maximal;
            _appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint, false);
        }
        else if (width >= 550 && width < 850 && _currentNavigationBarDisplayMode != ElaNavigationType::Compact)
        {
            _navigationBar->setDisplayMode(ElaNavigationType::Compact);
            _currentNavigationBarDisplayMode = ElaNavigationType::Compact;
            _appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint, false);
        }
        else if (width < 550 && _currentNavigationBarDisplayMode != ElaNavigationType::Minimal)
        {
            _navigationBar->setDisplayMode(ElaNavigationType::Minimal);
            _currentNavigationBarDisplayMode = ElaNavigationType::Minimal;
            _appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint);
        }
        _isNavigationBarExpanded = false;
    }
}
