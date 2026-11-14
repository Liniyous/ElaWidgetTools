#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELANAVIGATIONBARPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELANAVIGATIONBARPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaSuggestBox.h"
#include "ElaWidgetToolsDef.h"
#include <QMap>
#include <QObject>
#include <QPersistentModelIndex>
class QAbstractItemView;
class QPropertyAnimation;
class QTimer;
class QVariantAnimation;
class ElaMenu;
class QVBoxLayout;

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
    ELA_PIMPL_D(ElaNavigationBar)
    ELA_PROPERTY_D(bool, IsTransparent)
    ELA_PROPERTY_D(bool, IsAllowPageOpenInNewWindow)
    ELA_PROPERTY_D(int, NavigationBarWidth)

public:
    explicit ElaNavigationBarPrivate(QObject* parent = nullptr);
    ~ElaNavigationBarPrivate() override;
    Q_SLOT void onNavigationOpenNewWindow(const QString& nodeKey);

    //核心跳转逻辑
    void onTreeViewClicked(const QModelIndex& index, bool isLogRoute = true, bool isRouteBack = false, bool isScrollToNode = false);
    void onFooterViewClicked(const QModelIndex& index, bool isLogRoute = true, bool isRouteBack = false);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    friend class ElaNavigationView;
    ElaThemeType::ThemeMode _themeMode;
    QList<ElaSuggestBox::SuggestData> _suggestDataList;
    QMap<QString, const QMetaObject*> _pageMetaMap;
    QMap<QString, int> _pageNewWindowCountMap;
    QMap<ElaNavigationNode*, ElaMenu*> _compactMenuMap;
    QVBoxLayout* _userCardLayout{nullptr};

    ElaIconButton* _userButton{nullptr};
    ElaNavigationModel* _navigationModel{nullptr};
    ElaNavigationView* _navigationView{nullptr};
    ElaBaseListView* _footerView{nullptr};
    ElaFooterModel* _footerModel{nullptr};
    ElaFooterDelegate* _footerDelegate{nullptr};
    ElaInteractiveCard* _userCard{nullptr};
    bool _isShowUserCard{true};

    QStringList _lastExpandedNodeKeys;
    QPersistentModelIndex _pendingScrollIndex;
    QTimer* _scrollTimer{nullptr};
    QPropertyAnimation* _scrollAnimation{nullptr};
    QVariantAnimation* _navigationBarWidthAnimation{nullptr};

    ElaNavigationType::NavigationDisplayMode _currentDisplayMode{ElaNavigationType::NavigationDisplayMode::Maximal};
    void _initNodeModelIndex(const QModelIndex& parentIndex);
    void _resetNodeSelected();
    void _activatePage(ElaNavigationNode* node, bool isLogRoute, bool isRouteBack, bool isScrollToNode);
    ElaNavigationNode* _getSelectedNode() const;
    ElaNavigationNode* _getVisibleNode(ElaNavigationNode* node) const;
    void _setSelectedNode(ElaNavigationNode* node);
    static void _updateSelection(QAbstractItemView* view, ElaNavigationNode* node);
    void _startSelectionAnimation(ElaNavigationNode* previousNode, ElaNavigationNode* selectedNode, bool isFooter);
    void _ensureNodeVisible(ElaNavigationNode* node);
    void _setNodeExpanded(ElaNavigationNode* node, bool isExpanded);
    void _updateCategoryVisibility();
    void _expandOrCollapseExpanderNode(ElaNavigationNode* node, bool isExpand);

    void _addStackedPage(QWidget* page, QString pageKey);
    void _addFooterPage(QWidget* page, QString footKey);

    void _raiseNavigationBar();
    void _smoothScrollNavigationView(const QModelIndex& index);
    void _scrollToPendingNode();

    void _doComponentAnimation(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _handleNavigationExpandState(bool isSave, bool isAnimation);
    void _updateDisplayLayout();

    void _doNavigationBarWidthAnimation(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELANAVIGATIONBARPRIVATE_H_
