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
class ElaCentralStackedWidget;
class QHBoxLayout;
class QVBoxLayout;
class ElaThemeAnimationWidget;
class ElaWindowPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaWindow)
    Q_PROPERTY_CREATE_D(int, ThemeChangeTime)
    Q_PROPERTY_CREATE_D(ElaNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
    Q_PROPERTY_CREATE_D(ElaWindowType::StackSwitchMode, StackSwitchMode)
public:
    explicit ElaWindowPrivate(QObject* parent = nullptr);
    ~ElaWindowPrivate() override;
    Q_SLOT void onNavigationButtonClicked();
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
    Q_SLOT void onThemeReadyChange();
    Q_SLOT void onDisplayModeChanged();
    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);
    Q_SLOT void onNavigationNodeClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, bool isRouteBack);
    Q_SLOT void onNavigationNodeAdded(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);
    Q_SLOT void onNavigationNodeRemoved(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey);

private:
    ElaThemeType::ThemeMode _themeMode;
    QImage _lightBaseImage;
    QImage _darkBaseImage;
    bool _isWindowClosing{false};

    bool _isInitFinished{false};
    ElaEvent* _focusEvent{nullptr};
    ElaNavigationBar* _navigationBar{nullptr};
    ElaCentralStackedWidget* _centerStackedWidget{nullptr};
    ElaAppBar* _appBar{nullptr};
    QHBoxLayout* _centerLayout{nullptr};
    int _contentsMargins{5};
    bool _isNavigationDisplayModeChanged{false};
    bool _isNavigationEnable{true};
    bool _isNavigationBarExpanded{false};
    bool _isWMClickedAnimationFinished{true};
    ElaThemeAnimationWidget* _animationWidget{nullptr};

    ElaNavigationType::NavigationDisplayMode _currentNavigationBarDisplayMode{ElaNavigationType::Maximal};

    QMap<QString, QWidget*> _routeMap; // key__nodeKey title可以一致  value__Page
    int _navigationTargetIndex{0};
    qreal _distance(QPoint point1, QPoint point2);
    void _resetWindowLayout(bool isAnimation);
    void _doNavigationDisplayModeChange();
};

#endif // ELAWINDOWPRIVATE_H
