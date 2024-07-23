#include "ElaListView.h"

#include "DeveloperComponents/ElaListViewDelegate.h"
#include "ElaScrollBar.h"
ElaListView::ElaListView(QWidget* parent)
    : QListView(parent)
{
    setStyleSheet(
        "QListView{background-color: transparent;border:0px;}"
        "QListView::item{border:none;}");
    setVerticalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollBar(new ElaScrollBar(this));
    setSelectionMode(QAbstractItemView::NoSelection);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setItemDelegate(new ElaListViewDelegate(this));
}

ElaListView::~ElaListView()
{
}
