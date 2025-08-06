#include "ElaTableView.h"

#include <QHeaderView>
#include <QMouseEvent>

#include "DeveloperComponents/ElaTableViewStyle.h"
#include "ElaScrollBar.h"
#include "ElaTableViewPrivate.h"
ElaTableView::ElaTableView(QWidget* parent)
    : QTableView(parent), d_ptr(new ElaTableViewPrivate())
{
    Q_D(ElaTableView);
    d->q_ptr = this;
    setMouseTracking(true);
    setObjectName("ElaTableView");
    setStyleSheet(
        "QTableView{background-color:transparent;}"
        "QHeaderView{background-color:transparent;border:0px;}");
    setShowGrid(false);
    setVerticalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollBar(new ElaScrollBar(this));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    d->_tableViewStyle = new ElaTableViewStyle(style());
    setStyle(d->_tableViewStyle);
}

ElaTableView::~ElaTableView()
{
    Q_D(ElaTableView);
    delete d->_tableViewStyle;
}

void ElaTableView::setHeaderMargin(int headerMargin)
{
    Q_D(ElaTableView);
    if (headerMargin >= 0)
    {
        d->_tableViewStyle->setHeaderMargin(headerMargin);
        doItemsLayout();
    }
}

int ElaTableView::getHeaderMargin() const
{
    Q_D(const ElaTableView);
    return d->_tableViewStyle->getHeaderMargin();
}

void ElaTableView::showEvent(QShowEvent* event)
{
    Q_EMIT tableViewShow();
    QTableView::showEvent(event);
}

void ElaTableView::hideEvent(QHideEvent* event)
{
    Q_EMIT tableViewHide();
    QTableView::hideEvent(event);
}

void ElaTableView::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaTableView);
    if (selectionBehavior() == QAbstractItemView::SelectRows)
    {
        d->_tableViewStyle->setCurrentHoverRow(indexAt(event->pos()).row());
        update();
    }
    QTableView::mouseMoveEvent(event);
}

void ElaTableView::leaveEvent(QEvent* event)
{
    Q_D(ElaTableView);
    if (selectionBehavior() == QAbstractItemView::SelectRows)
    {
        d->_tableViewStyle->setCurrentHoverRow(-1);
        update();
    }
    QTableView::leaveEvent(event);
}
