#include "ElaRibbonBarView.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaMenu.h"
#include "ElaRibbonBar.h"
#include "ElaRibbonBarDelegate.h"
#include "ElaRibbonBarModel.h"
#include "ElaScrollBar.h"
#include "ElaTheme.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QScroller>

ElaRibbonBarView::ElaRibbonBarView(QWidget* parent)
    : QListView(parent)
{
    _ribbonBar = dynamic_cast<ElaRibbonBar*>(parent);
    _pRibbonBarModel = nullptr;
    _pRibbonBarDelegate = nullptr;
    setObjectName("ElaRibbonBarView");
    setStyleSheet(
        "ElaRibbonBarView{background-color: transparent;border:0px;}"
        "ElaRibbonBarView::item{border:none;}");
    setAutoScroll(false);
    setMouseTracking(true);
    setVerticalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setFlow(QListView::LeftToRight);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QScroller::grabGesture(viewport(), QScroller::LeftMouseButtonGesture);
    QScroller* scroller = QScroller::scroller(viewport());
    QScrollerProperties properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    scroller->setScrollerProperties(properties);
    connect(scroller, &QScroller::stateChanged, this, [=](QScroller::State newState) {
        if (!_pRibbonBarDelegate)
        {
            return;
        }
        if (newState == QScroller::Pressed)
        {
            _pRibbonBarDelegate->setPressIndex(indexAt(mapFromGlobal(QCursor::pos())));
            viewport()->update();
        }
        else if (newState == QScroller::Scrolling || newState == QScroller::Inactive)
        {
            _pRibbonBarDelegate->setPressIndex(QModelIndex());
        }
    });

    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaRibbonBarView::~ElaRibbonBarView()
{
}

void ElaRibbonBarView::mousePressEvent(QMouseEvent* event)
{
    _pRibbonBarDelegate->setPressIndex(indexAt(event->pos()));
    viewport()->update();
    QListView::mousePressEvent(event);
}

void ElaRibbonBarView::mouseReleaseEvent(QMouseEvent* event)
{
    QListView::mouseReleaseEvent(event);
    _pRibbonBarDelegate->setPressIndex(QModelIndex{});
    viewport()->update();
    // 触发Action
    auto modelIndex = indexAt(event->pos());
    if (modelIndex.isValid())
    {
        auto action = _pRibbonBarModel->getRibbonAction(modelIndex.row());
        if (!action || !action->isEnabled())
        {
            return;
        }
        auto actionMenu = action->menu();
        auto groupName = action->property("ElaRibbonGroupName").toString();
        action->trigger();
        Q_EMIT _ribbonBar->ribbonActionTriggered(_ribbonBar->getCurrentRibbonPage(), groupName, action);
        if (actionMenu)
        {
            QRect menuRect = actionMenu->rect();
            QRect indexRect = visualRect(modelIndex);
            if (groupName.isEmpty())
            {
                actionMenu->exec(mapToGlobal(QPoint(indexRect.center().x() - menuRect.width() / 2, indexRect.bottom() - 5)));
            }
            else
            {
                actionMenu->exec(mapToGlobal(QPoint(indexRect.center().x() - menuRect.width() / 2, indexRect.bottom() - _pRibbonBarDelegate->getBottomMargin() - 5)));
            }
        }
    }
}

void ElaRibbonBarView::mouseDoubleClickEvent(QMouseEvent* event)
{
    _pRibbonBarDelegate->setPressIndex(indexAt(event->pos()));
    viewport()->update();
    QListView::mouseDoubleClickEvent(event);
}

void ElaRibbonBarView::wheelEvent(QWheelEvent* event)
{
    QListView::wheelEvent(event);
    event->accept();
}

void ElaRibbonBarView::paintEvent(QPaintEvent* event)
{
    QListView::paintEvent(event);
    if (!_pRibbonBarModel)
    {
        return;
    }
    QPainter painter(viewport());
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QFont textFont = painter.font();
    textFont.setPixelSize(eApp->getFontPixelSize() - 2);
    painter.setFont(textFont);
    // 底边绘制
    auto& groupDataList = _pRibbonBarModel->getRibbonGroupData();
    int indexRow = 0;
    for (const auto& groupData: groupDataList)
    {
        // 组名为空为根Action
        if (!groupData.groupName.isEmpty())
        {
            auto startIndexRect = visualRect(_pRibbonBarModel->index(indexRow, 0));
            auto endIndexRect = visualRect(_pRibbonBarModel->index(indexRow + groupData.ribbonDataList.count() - 1, 0));
            // painter.setPen(ElaThemeColor(_themeMode, BasicBorder));
            // painter.setBrush(Qt::NoBrush);
            // painter.drawRect(QRect(startIndexRect.x(), startIndexRect.y(), endIndexRect.right() - startIndexRect.x(), startIndexRect.height()));
            auto groupRect = QRect(startIndexRect.x(), startIndexRect.bottom() - _pRibbonBarDelegate->getBottomMargin(), endIndexRect.right() - startIndexRect.x(), _pRibbonBarDelegate->getBottomMargin());
            painter.setPen(Qt::NoPen);
            painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(_themeMode, BasicBaseAlpha), "Background"));
            painter.drawRect(groupRect);
            // 组名绘制
            painter.setPen(ElaThemeColor(_themeMode, BasicText));
            painter.drawText(groupRect, Qt::AlignCenter, groupData.groupName);
        }
        indexRow += groupData.ribbonDataList.count();
    }
    painter.restore();
}