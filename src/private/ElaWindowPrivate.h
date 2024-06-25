#ifndef ELAWINDOWPRIVATE_H
#define ELAWINDOWPRIVATE_H

#include <QLinearGradient>
#include <QMap>
#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaEvent;
class ElaWindow;
class ElaNavigationBar;
class ElaAppBar;
class QStackedWidget;
class QHBoxLayout;
class QVBoxLayout;
class QParallelAnimationGroup;
class ElaThemeAnimationWidget;
class ElaWindowPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaWindow)
    Q_PROPERTY_CREATE_D(int, ThemeChangeTime)
    Q_PROPERTY_CREATE_D(ElaNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)

public:
    explicit ElaWindowPrivate(QObject* parent = nullptr);
    ~ElaWindowPrivate();
    Q_SLOT void onNavigationButtonClicked();
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
    Q_SLOT void onThemeReadyChange();
    Q_SLOT void onDisplayModeChanged();
    Q_SLOT void onThemeModeChanged(ElaApplicationType::ThemeMode themeMode);

private:
    friend class ElaNavigationBar;
    friend class ElaNavigationBarPrivate;
    ElaNavigationType::NavigationDisplayMode _currentNavigationBarDisplayMode{ElaNavigationType::NavigationDisplayMode::Maximal};
    ElaEvent* _focusEvent{nullptr};
    ElaNavigationBar* _navigationBar{nullptr};
    QStackedWidget* _centerStackedWidget{nullptr};
    ElaAppBar* _appBar{nullptr};
    QLinearGradient* _windowLinearGradient{nullptr};
    QHBoxLayout* _centerLayout{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    bool _isNavigationBarExpanded{false};
    bool _isNavagationAnimationFinished{true};
    int _contentsMargins{5};
    int _compactBarWidth{45};
    bool _isNavigationEnable{true};
    ElaThemeAnimationWidget* _animationWidget{nullptr};

    qreal _distance(QPoint point1, QPoint point2);
    void _adjustNavigationSize();
    void _resetWindowLayout(bool isAnimation);
};

#endif // ELAWINDOWPRIVATE_H
