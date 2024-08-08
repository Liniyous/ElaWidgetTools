#ifndef ELASCROLLBARPRIVATE_H
#define ELASCROLLBARPRIVATE_H

#include <QObject>

#include "stdafx.h"
class QTimer;
class QPropertyAnimation;
class ElaScrollBar;
class ElaScrollBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaScrollBar)
    Q_PROPERTY_CREATE_D(bool, isAnimation)
    Q_PROPERTY_CREATE(int, TargetMaximum)
public:
    explicit ElaScrollBarPrivate(QObject* parent = nullptr);
    ~ElaScrollBarPrivate();
    Q_SLOT void onRangeChanged(int min, int max);

private:
    QTimer* _expandTimer{nullptr};
    bool _isExpand{false};
    QPropertyAnimation* _slideSmoothAnimation{nullptr};
    int _scrollValue{-1};
    int _lastHorizontalDeltaAngle{-120};
    int _lastVerticalDeltaAngle{-120};
    void _scroll(Qt::KeyboardModifiers modifiers, int value);
    int _pixelPosToRangeValue(int pos) const;
};

#endif // ELASCROLLBARPRIVATE_H
