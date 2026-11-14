#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATABWIDGETPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATABWIDGETPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"

#include <QList>
#include <QObject>
#include <QPointer>
#include <QSize>

class ElaTabWidget;
class ElaTabBar;
class ElaTabDropIndicator;
class QStackedWidget;
class QPixmap;

class ElaTabWidgetPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaTabWidget)
    ELA_PROPERTY_D(bool, IsTabTransparent)
    ELA_PROPERTY_D(bool, IsContainerAcceptDrops)
    ELA_PROPERTY_D(QSize, FloatWidgetSize)
public:
    explicit ElaTabWidgetPrivate(QObject* parent = nullptr);
    ElaTabBar* getTabBar(int index) const;
    int getBarIndex(int index) const;
    void updateTabRows();
    void startTabDrag(int index);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QPixmap _getTabDragPixmap(int index) const;
    QRect _getContentRect() const;
    void _showDropIndicator(ElaTabWidgetType::DropArea area);

    QList<QWidget*> _pages;                       // 固定标签在前的页面顺序
    ElaTabBar* _tabBar{nullptr};                  // 普通标签行
    ElaTabBar* _pinnedTabBar{nullptr};            // 固定标签行
    QStackedWidget* _pageStackedWidget{nullptr};  // 两行标签共用的页面堆栈
    QPointer<QObject> _dragScope;                 // 可互相转移页面的容器范围
    QPointer<ElaTabWidget> _returnTabWidget;      // 浮窗关闭时归还页面的容器
    bool _isTransferManaged{false};               // 由外层处理跨组转移和复制
    bool _isDragCanceled{false};                  // 当前拖动是否由退出键取消
    bool _isDropIndicatorEnabled{false};          // 是否绘制拖放覆盖效果
    bool _isSplitDropEnabled{false};              // 是否提供上下左右分屏落点
    ElaTabDropIndicator* _dropIndicator{nullptr}; // 当前标签组独立的拖放覆盖
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATABWIDGETPRIVATE_H_
