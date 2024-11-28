#include "ElaWindowPrivate.h"

#include <QApplication>
#include <QPropertyAnimation>
#include <QTimer>
#include <QVBoxLayout>
#include <QtMath>

#include "ElaAppBar.h"
#include "ElaAppBarPrivate.h"
#include "ElaApplication.h"
#include "ElaCentralStackedWidget.h"
#include "ElaNavigationBar.h"
#include "ElaTheme.h"
#include "ElaThemeAnimationWidget.h"
#include "ElaWindow.h"
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
        _navigationBar->resize(_navigationBar->width(), _centerStackedWidget->height() + 1);
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
                    _navigationBar->setIsTransparent(true);
                    _isWMClickedAnimationFinished = true;
                    _resetWindowLayout(false);
                    navigationMoveAnimation->deleteLater();
                }
            });
            connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [=]() {
                _navigationBar->setIsTransparent(true);
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
        _animationWidget->setNewWindowBackground(q->grab(q->rect()).toImage());
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
    if (!eApp->getIsEnableMica())
    {
        QPalette palette = q->palette();
        palette.setBrush(QPalette::Window, ElaThemeColor(_themeMode, WindowBase));
        q->setPalette(palette);
    }
}

void ElaWindowPrivate::onNavigationNodeClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey)
{
    int nodeIndex = _routeMap.value(nodeKey);
    if (nodeIndex == -1)
    {
        // 页脚没有绑定页面
        return;
    }
    if (_navigationTargetIndex == nodeIndex || _centerStackedWidget->count() <= nodeIndex)
    {
        return;
    }
    _navigationTargetIndex = nodeIndex;
    QTimer::singleShot(180, this, [=]() {
        QWidget* currentWidget = _centerStackedWidget->widget(nodeIndex);
        _centerStackedWidget->setCurrentIndex(nodeIndex);
        QPropertyAnimation* currentWidgetAnimation = new QPropertyAnimation(currentWidget, "pos");
        currentWidgetAnimation->setEasingCurve(QEasingCurve::OutCubic);
        currentWidgetAnimation->setDuration(300);
        QPoint currentWidgetPos = currentWidget->pos();
        currentWidgetAnimation->setEndValue(currentWidgetPos);
        currentWidgetPos.setY(currentWidgetPos.y() + 80);
        currentWidgetAnimation->setStartValue(currentWidgetPos);
        currentWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    });
}

void ElaWindowPrivate::onNavigationNodeAdded(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page)
{
    if (nodeType == ElaNavigationType::PageNode)
    {
        _routeMap.insert(nodeKey, _centerStackedWidget->count());
        _centerStackedWidget->addWidget(page);
    }
    else
    {
        if (page)
        {
            _routeMap.insert(nodeKey, _centerStackedWidget->count());
            _centerStackedWidget->addWidget(page);
        }
        else
        {
            _routeMap.insert(nodeKey, -1);
        }
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
            _navigationBar->setDisplayMode(ElaNavigationType::Minimal, false);
            _centerLayout->addWidget(_navigationBar);
            _centerLayout->addWidget(_centerStackedWidget);
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
