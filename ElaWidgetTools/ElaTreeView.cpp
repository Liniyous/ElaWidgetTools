#include "ElaTreeView.h"

#include "ElaScrollBar.h"
#include "ElaTreeViewPrivate.h"
#include "ElaTreeViewStyle.h"
ElaTreeView::ElaTreeView(QWidget* parent)
    : QTreeView(parent), d_ptr(new ElaTreeViewPrivate())
{
    Q_D(ElaTreeView);
    d->q_ptr = this;
    setObjectName("ElaTreeView");
    setStyleSheet(
        "#ElaTreeView{background-color:transparent;}"
        "QHeaderView{background-color:transparent;border:0px;}");

    setAnimated(true);
    setMouseTracking(true);

    ElaScrollBar* hScrollBar = new ElaScrollBar(this);
    hScrollBar->setIsAnimation(true);
    connect(hScrollBar, &ElaScrollBar::rangeAnimationFinished, this, [=]() {
        doItemsLayout();
    });
    setHorizontalScrollBar(hScrollBar);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ElaScrollBar* vScrollBar = new ElaScrollBar(this);
    vScrollBar->setIsAnimation(true);
    connect(vScrollBar, &ElaScrollBar::rangeAnimationFinished, this, [=]() {
        doItemsLayout();
    });
    setVerticalScrollBar(vScrollBar);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    d->_treeViewStyle = new ElaTreeViewStyle(style());
    setStyle(d->_treeViewStyle);
}

ElaTreeView::~ElaTreeView()
{
    Q_D(ElaTreeView);
    delete d->_treeViewStyle;
}

void ElaTreeView::setItemHeight(int itemHeight)
{
    Q_D(ElaTreeView);
    if (itemHeight > 0)
    {
        d->_treeViewStyle->setItemHeight(itemHeight);
        doItemsLayout();
    }
}

int ElaTreeView::getItemHeight() const
{
    Q_D(const ElaTreeView);
    return d->_treeViewStyle->getItemHeight();
}

void ElaTreeView::setHeaderMargin(int headerMargin)
{
    Q_D(ElaTreeView);
    if (headerMargin >= 0)
    {
        d->_treeViewStyle->setHeaderMargin(headerMargin);
        doItemsLayout();
    }
}

int ElaTreeView::getHeaderMargin() const
{
    Q_D(const ElaTreeView);
    return d->_treeViewStyle->getHeaderMargin();
}
