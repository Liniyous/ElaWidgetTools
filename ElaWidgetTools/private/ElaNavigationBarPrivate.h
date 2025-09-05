#ifndef ELANAVIGATIONBARPRIVATE_H
#define ELANAVIGATIONBARPRIVATE_H

#include <QMap>
#include <QObject>

#include "ElaDef.h"
class QLayout;
class ElaMenu;
class QVBoxLayout;
class QHBoxLayout;
class QLinearGradient;

class ElaNavigationBar;
class ElaNavigationNode;
class ElaNavigationModel;
class ElaNavigationView;
class ElaNavigationDelegate;
class ElaSuggestBox;
class ElaInteractiveCard;

class ElaBaseListView;
class ElaFooterModel;
class ElaFooterDelegate;
class ElaIconButton;
class ElaToolButton;
class ElaNavigationBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaNavigationBar)
    Q_PROPERTY_CREATE_D(bool, IsTransparent)
    Q_PROPERTY_CREATE_D(bool, IsAllowPageOpenInNewWindow)
    Q_PROPERTY_CREATE_D(int, NavigationBarWidth)
    Q_PROPERTY_CREATE(int, NavigationViewWidth);

public:
    explicit ElaNavigationBarPrivate(QObject* parent = nullptr);
    ~ElaNavigationBarPrivate() override;
    Q_SLOT void onNavigationButtonClicked();
    Q_SLOT void onNavigationOpenNewWindow(QString nodeKey);

    Q_INVOKABLE void onNavigationRouteBack(QVariantMap routeData);

    //核心跳转逻辑
    void onTreeViewClicked(const QModelIndex& index, bool isLogRoute = true, bool isRouteBack = false);
    void onFooterViewClicked(const QModelIndex& index, bool isLogRoute = true, bool isRouteBack = false);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event);

private:
    friend class ElaNavigationView;
    friend class ElaNavigationStyle;
    ElaThemeType::ThemeMode _themeMode;
    QMap<QString, QString> _suggestKeyMap;
    QMap<QString, const QMetaObject*> _pageMetaMap;
    QMap<QString, int> _pageNewWindowCountMap;
    QMap<ElaNavigationNode*, ElaMenu*> _compactMenuMap;
    QVBoxLayout* _navigationButtonLayout{nullptr};
    QHBoxLayout* _navigationSuggestLayout{nullptr};
    QVBoxLayout* _userButtonLayout{nullptr};

    ElaIconButton* _userButton{nullptr};
    ElaToolButton* _searchButton{nullptr};
    ElaToolButton* _navigationButton{nullptr};
    ElaNavigationModel* _navigationModel{nullptr};
    ElaNavigationView* _navigationView{nullptr};
    ElaBaseListView* _footerView{nullptr};
    ElaFooterModel* _footerModel{nullptr};
    ElaFooterDelegate* _footerDelegate{nullptr};
    ElaSuggestBox* _navigationSuggestBox{nullptr};
    ElaInteractiveCard* _userCard{nullptr};
    bool _isShowUserCard{true};

    QList<ElaNavigationNode*> _lastExpandedNodesList;

    ElaNavigationType::NavigationDisplayMode _currentDisplayMode{ElaNavigationType::NavigationDisplayMode::Maximal};
    void _initNodeModelIndex(const QModelIndex& parentIndex);
    void _resetNodeSelected();
    void _expandSelectedNodeParent();
    void _expandOrCollpaseExpanderNode(ElaNavigationNode* node, bool isExpand);

    void _addStackedPage(QWidget* page, QString pageKey);
    void _addFooterPage(QWidget* page, QString footKey);

    void _raiseNavigationBar();

    void _doComponentAnimation(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _handleNavigationExpandState(bool isSave);
    void _handleMaximalToCompactLayout();
    void _handleCompactToMaximalLayout();
    void _resetLayout();

    void _doNavigationBarWidthAnimation(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _doNavigationViewWidthAnimation(bool isAnimation);
    void _doNavigationButtonAnimation(bool isCompact, bool isAnimation);
    void _doSearchButtonAnimation(bool isCompact, bool isAnimation);
    void _doUserButtonAnimation(bool isCompact, bool isAnimation);
};

#endif // ELANAVIGATIONBARPRIVATE_H
