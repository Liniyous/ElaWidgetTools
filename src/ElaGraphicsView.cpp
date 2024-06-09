#include "ElaGraphicsView.h"

#include <QEvent>
#include <QScrollBar>
#include <QWheelEvent>

#include "ElaScrollBar.h"
ElaGraphicsView::ElaGraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
    _initStyle();
}

ElaGraphicsView::ElaGraphicsView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent)
{
    _initStyle();
}

ElaGraphicsView::~ElaGraphicsView()
{
}

void ElaGraphicsView::wheelEvent(QWheelEvent* event)
{
    // qDebug() << transform().m11();
    if (event->modifiers() == Qt::CTRL)
    {
        // 放大
        if ((event->angleDelta().y() > 0) && transform().m11() <= 5)
        {
            this->scale(1.1, 1.1);
            return;
        }
        else if ((event->angleDelta().y() < 0) && transform().m11() >= 0.15)
        {
            this->scale(1.0 / 1.1, 1.0 / 1.1);
            return;
        }
    }
    QGraphicsView::wheelEvent(event);
}

void ElaGraphicsView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Alt)
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    QGraphicsView::keyPressEvent(event);
}

void ElaGraphicsView::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Alt)
    {
        setDragMode(QGraphicsView::RubberBandDrag);
    }
    QGraphicsView::keyReleaseEvent(event);
}

void ElaGraphicsView::_initStyle()
{
    setDragMode(QGraphicsView::RubberBandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setMouseTracking(true);
    setObjectName("ElaGraphicsView");
    setStyleSheet("#ElaGraphicsView{background-color:white;border:0px;}");
    setVerticalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollBar(new ElaScrollBar(this));
}
