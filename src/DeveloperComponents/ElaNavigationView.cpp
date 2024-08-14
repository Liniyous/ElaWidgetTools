#include "ElaNavigationView.h"

#include <QDebug>
#include <QHeaderView>
#include <QModelIndex>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QScroller>

#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationStyle.h"
#include "ElaScrollBar.h"
ElaNavigationView::ElaNavigationView(QWidget* parent)
    : QTreeView(parent)
{
    setObjectName("ElaNavigationView");
    setStyleSheet("#ElaNavigationView{background-color:transparent;}");
    setAnimated(true);
    setHeaderHidden(true);
    setRootIsDecorated(false);
    setExpandsOnDoubleClick(false);
    setMouseTracking(true);
    setSelectionMode(QAbstractItemView::NoSelection);
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ElaScrollBar* vScrollBar = new ElaScrollBar(this);
    vScrollBar->setisAnimation(true);
    connect(vScrollBar, &ElaScrollBar::rangeAnimationFinished, this, [=]() {
        this->viewport()->update();
    });
    setVerticalScrollBar(vScrollBar);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
    QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
#if (QT_VERSION < QT_VERSION_CHECK(6, 5, 3))
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
#endif
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    QScroller::scroller(this->viewport())->setScrollerProperties(properties);

    _navigationStyle = new ElaNavigationStyle(this->style());
    _navigationStyle->setNavigationView(this);
    setStyle(_navigationStyle);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &ElaNavigationView::customContextMenuRequested, this, &ElaNavigationView::onCustomContextMenuRequested);
}

ElaNavigationView::~ElaNavigationView()
{
}

void ElaNavigationView::navigationNodeStateChange(QVariantMap data)
{
    this->_navigationStyle->navigationNodeStateChange(data);
}

void ElaNavigationView::onCustomContextMenuRequested(const QPoint& pos)
{
    QModelIndex posIndex = indexAt(pos);
    if (!posIndex.isValid())
    {
        return;
    }
    ElaNavigationNode* posNode = static_cast<ElaNavigationNode*>(posIndex.internalPointer());
    if (!posNode->getIsExpanderNode())
    {
        //qDebug() << posNode;
    }
}

void ElaNavigationView::mouseReleaseEvent(QMouseEvent* event)
{
    QTreeView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid())
        {
            Q_EMIT navigationClicked(index);
        }
    }
}
