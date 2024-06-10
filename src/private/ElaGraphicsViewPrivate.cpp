#include "ElaGraphicsViewPrivate.h"

#include "ElaGraphicsView.h"
#include "ElaScrollBar.h"
ElaGraphicsViewPrivate::ElaGraphicsViewPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaGraphicsViewPrivate::~ElaGraphicsViewPrivate()
{
}

void ElaGraphicsViewPrivate::_initStyle()
{
    Q_Q(ElaGraphicsView);
    q->setDragMode(QGraphicsView::RubberBandDrag);
    q->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    q->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    q->setMouseTracking(true);
    q->setObjectName("ElaGraphicsView");
    q->setStyleSheet("#ElaGraphicsView{background-color:white;border:0px;}");
    q->setVerticalScrollBar(new ElaScrollBar(q));
    q->setHorizontalScrollBar(new ElaScrollBar(q));
}
