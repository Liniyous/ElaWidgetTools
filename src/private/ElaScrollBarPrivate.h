#ifndef ELASCROLLBARPRIVATE_H
#define ELASCROLLBARPRIVATE_H

#include <QAbstractScrollArea>
#include <QObject>
#include <QScrollBar>

#include "stdafx.h"
class QTimer;
class QPropertyAnimation;
class ElaScrollBar;
class ElaScrollBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaScrollBar)
    Q_PROPERTY_CREATE_D(bool, IsAnimation)
    Q_PROPERTY_CREATE_D(qreal, SpeedLimit)
    Q_PROPERTY_CREATE(int, TargetMaximum)
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
    int _horSpace{10};
    int _verSpace{10};
    void _scroll(Qt::KeyboardModifiers modifiers, int value);
    int _pixelPosToRangeValue(int pos) const;

    // 映射处理函数
    void _initAllConfig();
    void _handleScrollBarValueChanged(QScrollBar* scrollBar, int value);
    void _handleScrollBarRangeChanged(int min, int max);
    void _handleScrollBarGeometry();
};

#endif // ELASCROLLBARPRIVATE_H
