#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLBARPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLBARPRIVATE_H_

#include <QAbstractScrollArea>
#include <QObject>
#include <QScrollBar>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class QTimer;
class QPropertyAnimation;
class ElaScrollBar;
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
    QScrollBar* _originScrollBar{nullptr};
    QAbstractScrollArea* _originScrollArea{nullptr};
    QTimer* _expandTimer{nullptr};
    bool _isExpand{false};
    QPropertyAnimation* _slideSmoothAnimation{nullptr};
    int _scrollValue{-1};
    void _scroll(Qt::KeyboardModifiers modifiers, int value);
    int _pixelPosToRangeValue(int pos) const;

    // 映射处理函数
    void _initAllConfig();
    void _handleScrollBarValueChanged(QScrollBar* scrollBar, int value);
    void _handleScrollBarRangeChanged(int min, int max);
    void _handleScrollBarGeometry();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLBARPRIVATE_H_

