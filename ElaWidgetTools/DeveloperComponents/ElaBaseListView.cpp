#include "ElaBaseListView.h"

#include <QMouseEvent>

#include "ElaScrollBar.h"
ElaBaseListView::ElaBaseListView(QWidget* parent)
    : QListView(parent)
{
    setObjectName("ElaBaseListView");
    setStyleSheet(
        "ElaBaseListView{background-color: transparent;border:0px;}"
        "ElaBaseListView::item{border:none;}");
    setAutoScroll(false);
    setFocusPolicy(Qt::NoFocus);
    setVerticalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollBar(new ElaScrollBar(this));
    setSelectionMode(QAbstractItemView::NoSelection);
    setMouseTracking(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

ElaBaseListView::~ElaBaseListView()
{
}

void ElaBaseListView::wheelEvent(QWheelEvent* event)
{
    QListView::wheelEvent(event);
    event->accept();
}

void ElaBaseListView::mousePressEvent(QMouseEvent* event)
{
    Q_EMIT mousePress(indexAt(event->pos()));
    QListView::mousePressEvent(event);
}

void ElaBaseListView::mouseReleaseEvent(QMouseEvent* event)
{
    Q_EMIT mouseRelease(indexAt(event->pos()));
    QListView::mouseReleaseEvent(event);
}

void ElaBaseListView::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_EMIT mouseDoubleClick(indexAt(event->pos()));
    QListView::mouseDoubleClickEvent(event);
}
