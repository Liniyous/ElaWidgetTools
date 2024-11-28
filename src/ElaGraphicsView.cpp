#include "ElaGraphicsView.h"

#include <QEvent>
#include <QScrollBar>
#include <QWheelEvent>

#include "private/ElaGraphicsViewPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsView, qreal, MaxTransform);
Q_PROPERTY_CREATE_Q_CPP(ElaGraphicsView, qreal, MinTransform);
ElaGraphicsView::ElaGraphicsView(QWidget* parent)
    : QGraphicsView(parent), d_ptr(new ElaGraphicsViewPrivate())
{
    Q_D(ElaGraphicsView);
    d->q_ptr = this;
    d->_pMaxTransform = 5;
    d->_pMinTransform = 0.15;
    d->_initStyle();
}

ElaGraphicsView::ElaGraphicsView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent), d_ptr(new ElaGraphicsViewPrivate())
{
    Q_D(ElaGraphicsView);
    d->q_ptr = this;
    d->_pMaxTransform = 5;
    d->_pMinTransform = 0.15;
    d->_initStyle();
}

ElaGraphicsView::~ElaGraphicsView()
{
}

void ElaGraphicsView::wheelEvent(QWheelEvent* event)
{
    Q_D(ElaGraphicsView);
    if (event->modifiers() == Qt::CTRL)
    {
        // 放大
        if ((event->angleDelta().y() > 0) && transform().m11() <= d->_pMaxTransform)
        {
            this->scale(1.1, 1.1);
        }
        else if ((event->angleDelta().y() < 0) && transform().m11() >= d->_pMinTransform)
        {
            this->scale(1.0 / 1.1, 1.0 / 1.1);
        }
        return;
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
