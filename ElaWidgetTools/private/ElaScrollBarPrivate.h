#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLBARPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLBARPRIVATE_H_

#include <QAbstractScrollArea>
#include <QObject>
#include <QScrollBar>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"
class QTimer;
class QPropertyAnimation;
class ElaScrollBar;
class ElaScrollAnimation;
class ElaScrollBarStyle;
class ElaScrollBarPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaScrollBar)
    ELA_PROPERTY_D(bool, IsAnimation)
    ELA_PROPERTY_D(qreal, SpeedLimit)
    ELA_PROPERTY(int, TargetMaximum)
public:
    explicit ElaScrollBarPrivate(QObject* parent = nullptr);
    ~ElaScrollBarPrivate();
    Q_SLOT void onRangeChanged(int min, int max);

private:
    int _pScrollBarWidth{10}; // 滚动条厚度 默认10个逻辑像素
    QScrollBar* _originScrollBar{nullptr};
    QAbstractScrollArea* _originScrollArea{nullptr};
    QTimer* _expandTimer{nullptr};
    ElaScrollBarStyle* _scrollBarStyle{nullptr}; // 滑块宽度和透明度的绘制状态
    bool _isExpand{false};
    bool _isSmoothScroll{true};                    // 滚轮输入是否使用缓动动画
    ElaScrollAnimation* _scrollAnimation{nullptr}; // 滚动条与编辑器共用的像素惯性运动
    qreal _scrollScale{1};                         // 每个滚动条数值单位对应的像素数
    qreal _pixelRemainder{0};                      // 行滚动控件尚未累计到一个数值单位的像素输入
    bool _isUpdatingScroll{false};                 // 当前值由惯性运动更新
    bool _scroll(qreal movement);
    qreal _getScrollScale() const;
    int _pixelPosToRangeValue(int pos) const;

    // 映射处理函数
    void _initAllConfig();
    void _handleScrollBarValueChanged(QScrollBar* scrollBar, int value);
    void _handleScrollBarRangeChanged(int min, int max);
    void _handleScrollBarGeometry();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLBARPRIVATE_H_
