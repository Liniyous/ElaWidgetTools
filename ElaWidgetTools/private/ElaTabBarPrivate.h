#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATABBARPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATABBARPRIVATE_H_

#include "ElaPropertyMacro.h"

#include <QObject>
#include <QPoint>

class ElaTabBar;
class ElaTabBarStyle;
class ElaScrollBar;
class QTabBarPrivate;
class QTimer;

class ElaTabBarPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaTabBar)
public:
    explicit ElaTabBarPrivate(QObject* parent = nullptr);
    Q_SLOT void onScrollBarValueChanged(int position);
    void updateScrollRange();
    void setScrollPosition(int position, bool animated);
    void ensureTabVisible(int index);
    int getDropIndex(const QPoint& position) const;
    void updateDropPosition(const QPoint& position);
    void clearDropPosition();

private:
    ElaTabBarStyle* _style{nullptr};         // 标签绘制样式
    QTabBarPrivate* _tabBarPrivate{nullptr}; // Qt 标签滚动与子控件布局
    ElaScrollBar* _scrollBar{nullptr};       // 标签溢出时的顶部滚动条
    QTimer* _dragScrollTimer{nullptr};       // 拖动靠近边缘时连续滚动
    QPoint _dropPosition;                    // 当前拖放位置
    int _dropIndex{-1};                      // 插入标记对应的标签间隙
    int _dragScrollDirection{0};             // 拖放边缘滚动方向
    bool _isDragEnabled{false};              // 是否允许标签拖出当前行
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATABBARPRIVATE_H_
