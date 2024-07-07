#include "ElaNavigationSuggestView.h"

#include <QGraphicsOpacityEffect>
#include <QPainter>

#include "ElaApplication.h"
#include "ElaScrollBar.h"
ElaNavigationSuggestView::ElaNavigationSuggestView(QWidget* parent)
    : QListView(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet(
        "QListView{border-bottom-left-radius:8px;border-bottom-right-radius: 8px;padding-top:8px;padding-bottom:8px;background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #F3F7FA, stop: 1 #F8F8F8);border:0px;}"
        "QListView::item{height: 40px;}");
    setVerticalScrollBar(new ElaScrollBar(this));
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(0xDA, 0xDA, 0xDA));
    shadow->setBlurRadius(50);
    setGraphicsEffect(shadow);
}

ElaNavigationSuggestView::~ElaNavigationSuggestView()
{
}
