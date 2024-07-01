#ifndef ELANAVIGATIONBARPRIVATE_H
#define ELANAVIGATIONBARPRIVATE_H

#include <QMap>
#include <QObject>
#include <QQueue>

#include "Def.h"
#include "stdafx.h"
class QLayout;
class ElaMenu;
class QVBoxLayout;
class QHBoxLayout;
class QStackedWidget;
class QLinearGradient;

class ElaNavigationBar;
class ElaNavigationNode;
class ElaNavigationModel;
class ElaNavigationView;
class ElaNavigationDelegate;
class ElaNavigationSuggestBox;
class ElaInteractiveCard;

class ElaListView;
class ElaFooterModel;
class ElaFooterDelegate;
class ElaCompactModel;
class ElaCompactDelegate;

class ElaNavigationBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaNavigationBar)
public:
    explicit ElaNavigationBarPrivate(QObject* parent = nullptr);
    ~ElaNavigationBarPrivate();
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);
    Q_INVOKABLE void onNavigationRouteBack(QVariantMap routeData);

    //核心跳转逻辑
    void onTreeViewClicked(const QModelIndex& index, bool isLogRoute = true);
    void onFooterViewClicked(const QModelIndex& index, bool isLogRoute = true);
    void onCompactViewClicked(const QModelIndex& index);

private:
    QMap<ElaNavigationNode*, ElaMenu*> _compactMenuMap;
    QWidget* _maximalWidget{nullptr};
    QWidget* _compactWidget{nullptr};
    QHBoxLayout* _mainLayout{nullptr};
    ElaListView* _compactView{nullptr};
    ElaCompactModel* _compactModel{nullptr};
    ElaCompactDelegate* _compactDelegate{nullptr};

    QLinearGradient* _windowLinearGradient{nullptr};
    ElaNavigationModel* _navigationModel{nullptr};
    ElaNavigationView* _navigationView{nullptr};
    ElaNavigationDelegate* _navigationDelegate{nullptr};
    ElaListView* _footerView{nullptr};
    ElaFooterModel* _footerModel{nullptr};
    ElaFooterDelegate* _footerDelegate{nullptr};
    ElaNavigationSuggestBox* _navigationSuggestBox{nullptr};
    ElaInteractiveCard* _userCard{nullptr};

    ElaNavigationType::NavigationDisplayMode _currentDisplayMode{ElaNavigationType::NavigationDisplayMode::Maximal};
    void _resetNodeSelected();
    void _expandSelectedNodeParent();
    void _initNodeModelIndex(const QModelIndex& parentIndex);

    void _addStackedPage(QWidget* page, QString pageKey);
    void _addFooterPage(QWidget* page, QString footKey);

    void _raiseNavigationBar();
    void _switchContentLayout(bool direction);
    void _startContentWidgetAnimation(QPoint startPoint, QPoint endPoint, bool isAnimation);
    void _resetNavigationLayout(ElaNavigationType::NavigationDisplayMode displayMode);
};

#endif // ELANAVIGATIONBARPRIVATE_H
