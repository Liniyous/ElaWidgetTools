#ifndef ELASCROLLBARPRIVATE_H
#define ELASCROLLBARPRIVATE_H

#include <QObject>

#include "stdafx.h"

class QPropertyAnimation;
class ElaScrollBar;
class ElaScrollBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaScrollBar)
public:
    explicit ElaScrollBarPrivate(QObject* parent = nullptr);

private:
    QPropertyAnimation* _slideSmoothAnimation{nullptr};
    QPropertyAnimation* _rangeSmoothAnimation{nullptr};
    int _scrollValue{0};
    int _lastHorizontalDeltaAngle{-120};
    int _lastVerticalDeltaAngle{-120};
    int _lastMaximum{0};
    bool _isRangeAnimationFinished{true};
    void _initStyle();
    void _scroll(int value);
};

#endif // ELASCROLLBARPRIVATE_H
