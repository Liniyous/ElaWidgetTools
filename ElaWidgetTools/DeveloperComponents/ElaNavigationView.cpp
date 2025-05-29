#include "ElaNavigationView.h"

#include <QDebug>
#include <QHeaderView>
#include <QModelIndex>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QScroller>

#include "ElaMenu.h"
#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationStyle.h"
#include "ElaScrollBar.h"
#include "ElaToolTip.h"
ElaNavigationView::ElaNavigationView(QWidget* parent)
    : QTreeView(parent)
{
    setObjectName("ElaNavigationView");
    setStyleSheet("#ElaNavigationView{background-color:transparent;}");
    setAnimated(true);
    setHeaderHidden(true);
    setRootIsDecorated(false);
    setExpandsOnDoubleClick(false);
    setAutoScroll(false);
    setMouseTracking(true);
    setSelectionMode(QAbstractItemView::NoSelection);

    // 滚动条设置
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ElaScrollBar* vScrollBar = new ElaScrollBar(this);
    connect(vScrollBar, &ElaScrollBar::rangeAnimationFinished, this, [=]() {
        doItemsLayout();
    });
    setVerticalScrollBar(vScrollBar);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ElaScrollBar* floatVScrollBar = new ElaScrollBar(vScrollBar, this);
    floatVScrollBar->setIsAnimation(true);
    floatVScrollBar->installEventFilter(this);

    _navigationStyle = new ElaNavigationStyle(this->style());
    _navigationStyle->setNavigationView(this);
    setStyle(_navigationStyle);

    QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller* scroller = QScroller::scroller(this->viewport());
    QScrollerProperties properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    scroller->setScrollerProperties(properties);

    connect(scroller, &QScroller::stateChanged, this, [=](QScroller::State newstate) {
        if (newstate == QScroller::Pressed)
        {
            _navigationStyle->setPressIndex(indexAt(mapFromGlobal(QCursor::pos())));
            viewport()->update();
        }
        else if (newstate == QScroller::Scrolling || newstate == QScroller::Inactive)
        {
            _navigationStyle->setPressIndex(QModelIndex());
        }
    });

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &ElaNavigationView::customContextMenuRequested, this, &ElaNavigationView::onCustomContextMenuRequested);

    _compactToolTip = new ElaToolTip(this);
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
    if (!_pNavigationBarPrivate->_pIsAllowPageOpenInNewWindow)
    {
        return;
    }
    QModelIndex posIndex = indexAt(pos);
    if (!posIndex.isValid())
    {
        return;
    }
    ElaNavigationNode* posNode = static_cast<ElaNavigationNode*>(posIndex.internalPointer());
    if (!posNode->getIsExpanderNode())
    {
        ElaMenu menu;
        menu.setMenuItemHeight(27);
        QAction* openAction = menu.addElaIconAction(ElaIconType::ObjectGroup, "在新窗口中打开");
        connect(openAction, &QAction::triggered, this, [=]() {
            Q_EMIT navigationOpenNewWindow(posNode->getNodeKey());
        });
        menu.exec(mapToGlobal(pos));
    }
}

void ElaNavigationView::mouseMoveEvent(QMouseEvent* event)
{
    if (_pNavigationBarPrivate->_currentDisplayMode == ElaNavigationType::NavigationDisplayMode::Compact)
    {
        QModelIndex posIndex = indexAt(event->pos());
        if (!posIndex.isValid())
        {
            _compactToolTip->hide();
            return;
        }
        ElaNavigationNode* posNode = static_cast<ElaNavigationNode*>(posIndex.internalPointer());
        _compactToolTip->setToolTip(posNode->getNodeTitle());
        _compactToolTip->updatePos();
        _compactToolTip->show();
    }
    else
    {
        _compactToolTip->hide();
    }
    QTreeView::mouseMoveEvent(event);
}

void ElaNavigationView::mouseDoubleClickEvent(QMouseEvent* event)
{
    _navigationStyle->setPressIndex(indexAt(event->pos()));
    viewport()->update();
    QTreeView::mouseDoubleClickEvent(event);
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
        _navigationStyle->setPressIndex(QModelIndex());
    }
}

bool ElaNavigationView::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::MouseMove:
    case QEvent::HoverMove:
    {
        if (_pNavigationBarPrivate->_currentDisplayMode == ElaNavigationType::NavigationDisplayMode::Compact)
        {
            QModelIndex posIndex = indexAt(mapFromGlobal(QCursor::pos()));
            if (!posIndex.isValid())
            {
                _compactToolTip->hide();
                break;
            }
            ElaNavigationNode* posNode = static_cast<ElaNavigationNode*>(posIndex.internalPointer());
            _compactToolTip->setToolTip(posNode->getNodeTitle());
            _compactToolTip->updatePos();
            _compactToolTip->show();
        }
        else
        {
            _compactToolTip->hide();
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return QAbstractItemView::eventFilter(watched, event);
}
