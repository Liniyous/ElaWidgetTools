#ifndef ELANAVIGATIONBARPRIVATE_H
#define ELANAVIGATIONBARPRIVATE_H

#include "ElaDef.h"
#include "ElaSuggestBox.h"
#include <QMap>
#include <QObject>
class QLayout;
class ElaMenu;
class QVBoxLayout;
class QHBoxLayout;
class QLinearGradient;

class ElaNavigationBar;
class ElaNavigationNode;
class ElaNavigationModel;
class ElaNavigationView;
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
    Q_PROPERTY_CREATE(int, UserButtonSpacing);

public:
    explicit ElaNavigationBarPrivate(QObject* parent = nullptr);
    ~ElaNavigationBarPrivate() override;
    Q_SLOT void onNavigationOpenNewWindow(QString nodeKey);

    Q_INVOKABLE void onNavigationRoute(QVariantMap routeData);

    //核心跳转逻辑
    void onTreeViewClicked(const QModelIndex& index, bool isLogRoute = true, bool isRouteBack = false);
    void onFooterViewClicked(const QModelIndex& index, bool isLogRoute = true, bool isRouteBack = false);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event);

private:
    friend class ElaNavigationView;
    friend class ElaNavigationStyle;
    ElaThemeType::ThemeMode _themeMode;
    QList<ElaSuggestBox::SuggestData> _suggestDataList;
    QMap<QString, const QMetaObject*> _pageMetaMap;
    QMap<QString, int> _pageNewWindowCountMap;
    QMap<ElaNavigationNode*, ElaMenu*> _compactMenuMap;
    QVBoxLayout* _userCardLayout{nullptr};
    QVBoxLayout* _userButtonLayout{nullptr};

    ElaIconButton* _userButton{nullptr};
    ElaNavigationModel* _navigationModel{nullptr};
    ElaNavigationView* _navigationView{nullptr};
    ElaBaseListView* _footerView{nullptr};
    ElaFooterModel* _footerModel{nullptr};
    ElaFooterDelegate* _footerDelegate{nullptr};
    ElaInteractiveCard* _userCard{nullptr};
    bool _isShowUserCard{true};

    QList<ElaNavigationNode*> _lastExpandedNodesList;

    ElaNavigationType::NavigationDisplayMode _currentDisplayMode{ElaNavigationType::NavigationDisplayMode::Maximal};
    void _initNodeModelIndex(const QModelIndex& parentIndex);
    void _resetNodeSelected();
    void _expandSelectedNodeParent();
    void _expandOrCollapseExpanderNode(ElaNavigationNode* node, bool isExpand);

    void _addStackedPage(QWidget* page, QString pageKey);
    void _addFooterPage(QWidget* page, QString footKey);

    void _raiseNavigationBar();
    void _smoothScrollNavigationView(const QModelIndex& index);

    void _doComponentAnimation(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _handleNavigationExpandState(bool isSave);
    void _handleUserButtonLayout(bool isCompact);
    void _resetLayout();

    void _doNavigationBarWidthAnimation(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _doNavigationViewWidthAnimation(bool isAnimation);
    void _doUserButtonAnimation(bool isCompact, bool isAnimation);
};

#endif // ELANAVIGATIONBARPRIVATE_H
