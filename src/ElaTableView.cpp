#include "ElaTableView.h"

#include <QHeaderView>

#include "DeveloperComponents/ElaTableViewStyle.h"
#include "ElaScrollBar.h"
ElaTableView::ElaTableView(QWidget* parent)
    : QTableView(parent)
{
    setMouseTracking(true);
    setStyleSheet(
        "QTableView{background-color: transparent;border:0px;}"
        "QTableView::item{border:none;}");
    setShowGrid(false);
    setVerticalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollBar(new ElaScrollBar(this));
    this->verticalHeader()->setHidden(true);
    this->horizontalHeader()->setHidden(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setStyle(new ElaTableViewStyle(style()));
}

ElaTableView::~ElaTableView()
{
}
