#include "ElaWindowPrivate.h"

#include <QImage>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "ElaAppBar.h"
#include "ElaAppBarPrivate.h"
#include "ElaApplication.h"
#include "ElaNavigationBar.h"
#include "ElaThemeAnimationWidget.h"
#include "ElaWindow.h"
#include "private/ElaNavigationBarPrivate.h"
ElaWindowPrivate::ElaWindowPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaWindowPrivate::~ElaWindowPrivate()
{
}

void ElaWindowPrivate::onNavigationButtonClicked()
{
    int contentMargin = _contentsMargins;
    int appBarHeight = _appBar->height();
    if (_navigationBar->pos().x() == -45 || _navigationBar->pos().x() == -305)
    {
        _navigationBar->switchCompact(false);
        _navigationBar->move(-_navigationBar->width(), _navigationBar->pos().y());
        _navigationBar->resize(_navigationBar->width(), _centerStackedWidget->height());
        QPropertyAnimation* navigationMoveAnimation = new QPropertyAnimation(_navigationBar, "pos");
        navigationMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
        navigationMoveAnimation->setDuration(300);
        navigationMoveAnimation->setStartValue(_navigationBar->pos());
        navigationMoveAnimation->setEndValue(QPoint(contentMargin, appBarHeight + contentMargin));
        navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        _navigationBar->d_ptr->_raiseNavigationBar();
        _isNavigationBarExpanded = true;
    }
}

void ElaWindowPrivate::onWMWindowClickedEvent(QVariantMap data)
{
    Q_UNUSED(data)
    if (ElaApplication::containsCursorToItem(_navigationBar))
    {
        return;
    }
    if (_isNavigationBarExpanded)
    {
        QPropertyAnimation* navigationMoveAnimation = new QPropertyAnimation(_navigationBar, "pos");
        navigationMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
        navigationMoveAnimation->setDuration(300);
        navigationMoveAnimation->setStartValue(_navigationBar->pos());
        navigationMoveAnimation->setEndValue(QPoint(-_navigationBar->width() - 5, 35));
        navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        _isNavigationBarExpanded = false;
    }
}

void ElaWindowPrivate::onThemeReadyChange()
{
    Q_Q(ElaWindow);
    if (!_animationWidget->getIsAnimationFinished())
    {
        return;
    }
    ElaApplication* app = ElaApplication::getInstance();
    _animationWidget->setWindowBackground(q->grab(q->rect()).toImage());

    QPoint centerPos = q->mapFromGlobal(QCursor::pos());
    _animationWidget->setCenter(centerPos);
    qreal topLeftDis = _distance(centerPos, QPoint(0, 0));
    qreal topRightDis = _distance(centerPos, QPoint(q->width(), 0));
    qreal bottomLeftDis = _distance(centerPos, QPoint(0, q->height()));
    qreal bottomRightDis = _distance(centerPos, QPoint(q->width(), q->height()));
    QList<qreal> disList{topLeftDis, topRightDis, bottomLeftDis, bottomRightDis};
    std::sort(disList.begin(), disList.end());
    _animationWidget->setEndRadius(disList[3]);
    _animationWidget->resize(q->width(), q->height());
    _animationWidget->move(q->mapToGlobal(QPoint(0, 0)));
    _animationWidget->setIsMaximum(q->isMaximized());
    _animationWidget->startAnimation(_pThemeChangeTime);
    if (app->getThemeMode() == ElaApplicationType::Light)
    {
        _windowLinearGradient->setColorAt(0, QColor(0x1A, 0x1A, 0x1A));
        _windowLinearGradient->setColorAt(1, QColor(0x1A, 0x1A, 0x1A));
        app->setThemeMode(ElaApplicationType::Dark);
    }
    else
    {
        _windowLinearGradient->setColorAt(0, QColor(0xF2, 0xF2, 0xF9));
        _windowLinearGradient->setColorAt(1, QColor(0xF9, 0xEF, 0xF6));
        app->setThemeMode(ElaApplicationType::Light);
    }
}

void ElaWindowPrivate::onDisplayModeChanged()
{
    switch (_pNavigationBarDisplayMode)
    {
    case ElaNavigationType::Auto:
    {
        break;
    }
    case ElaNavigationType::Minimal:
    {
        _navigationBar->move(-_navigationBar->width() - 5, 35);
        if (_centerLayout->count() == 2)
        {
            _centerLayout->takeAt(0);
        }
        _navigationBar->setVisible(false);
        _appBar->setWindowButtonFlag(ElaAppBarType::NavigationButtonHint);
        break;
    }
    case ElaNavigationType::Maximal:
    {
        _navigationBar->setVisible(true);
        break;
    }
    case ElaNavigationType::Compact:
    {
        _navigationBar->switchCompact(true);
        break;
    }
    }
}

void ElaWindowPrivate::onThemeModeChanged(ElaApplicationType::ThemeMode themeMode)
{
    if (ElaApplication::getInstance()->getThemeMode() == ElaApplicationType::Light)
    {
        _windowLinearGradient->setColorAt(0, QColor(0xF2, 0xF2, 0xF9));
        _windowLinearGradient->setColorAt(1, QColor(0xF9, 0xEF, 0xF6));
    }
    else
    {
        _windowLinearGradient->setColorAt(0, QColor(0x1A, 0x1A, 0x1A));
        _windowLinearGradient->setColorAt(1, QColor(0x1A, 0x1A, 0x1A));
    }
}

qreal ElaWindowPrivate::_distance(QPoint point1, QPoint point2)
{
    return sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) + (point1.y() - point2.y()) * (point1.y() - point2.y()));
}

void ElaWindowPrivate::_adjustNavigationSize()
{
    Q_Q(ElaWindow);
    if (!_isNavagationAnimationFinished)
    {
        //不在布局中 动态调整高度 避免动画过程中显示不完全
        ElaNavigationBarPrivate* navigationBarPrivate = _navigationBar->d_func();
        _centerStackedWidget->resize(q->width() - _centerStackedWidget->x() - 5, q->height() - 40);
        _navigationBar->resize(_navigationBar->width(), _centerStackedWidget->height());
        navigationBarPrivate->_maximalWidget->resize(navigationBarPrivate->_maximalWidget->width(), _navigationBar->height());
        navigationBarPrivate->_compactWidget->resize(navigationBarPrivate->_compactWidget->width(), _navigationBar->height());
    }
}

void ElaWindowPrivate::_resetWindowLayout(bool isAnimation)
{
    if (isAnimation)
    {
        while (_centerLayout->count() > 0)
        {
            _centerLayout->takeAt(0);
        }
        if (_mainLayout->count() == 2)
        {
            _mainLayout->addStretch();
        }
    }
    else
    {
        _centerLayout->addWidget(_navigationBar);
        _centerLayout->addWidget(_centerStackedWidget);
        _mainLayout->takeAt(2);
    }
}
