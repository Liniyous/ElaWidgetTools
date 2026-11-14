#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELANAVIGATIONVIEW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELANAVIGATIONVIEW_H_

#include "ElaNavigationBarPrivate.h"
#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include <QObject>
#include <QPixmap>
#include <QTreeView>
class ElaScrollBar;
class ElaNavigationStyle;
class ElaToolTip;
class QVariantAnimation;
class ElaNavigationView : public QTreeView
{
    Q_OBJECT
    ELA_VARIABLE(const ElaNavigationBarPrivate*, NavigationBarPrivate)
public:
    explicit ElaNavigationView(QWidget* parent = nullptr);
    ~ElaNavigationView() override;
    void setModel(QAbstractItemModel* model) override;
    void setNodeExpanded(ElaNavigationNode* node, bool isExpanded);
    void startSelectionAnimation(ElaNavigationNode* previousNode, ElaNavigationNode* selectedNode);

    Q_SLOT void onCustomContextMenuRequested(const QPoint& pos);
Q_SIGNALS:
    Q_SIGNAL void navigationClicked(const QModelIndex& index);
    Q_SIGNAL void navigationOpenNewWindow(QString nodeKey);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    ElaNavigationStyle* _navigationStyle{nullptr};
    ElaToolTip* _compactToolTip{nullptr};
    QVariantAnimation* _expandAnimation{nullptr};
    QPersistentModelIndex _expandIndex; // 当前过渡所属的分组
    QPixmap _collapsedPixmap;           // 收起状态的视口快照
    QPixmap _expandedPixmap;            // 展开状态的视口快照
    int _expandTop{0};                  // 分组标题下沿的视口逻辑坐标
    int _expandHeight{0};               // 视口内可见子项的完整高度
    int _getExpandedHeight(const QModelIndex& index) const;
    void _finishExpandAnimation();
    void _doCompactToolTip();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELANAVIGATIONVIEW_H_
