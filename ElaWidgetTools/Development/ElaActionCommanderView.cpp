#include "ElaActionCommanderView.h"

#include "ElaActionCommander.h"
#include "ElaActionCommanderViewStyle.h"
#include "ElaMenu.h"
#include "ElaScrollBar.h"
#include "ElaTheme.h"

#include <QPainter>
#include <QPropertyAnimation>

ElaActionCommanderView::ElaActionCommanderView(QWidget* parent)
    : QTreeView(parent)
{
    setObjectName("ElaActionCommanderView");
    setStyleSheet("#ElaActionCommanderView{border:0px;background-color:transparent;}");
    setAnimated(true);
    setHeaderHidden(true);
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

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &ElaActionCommanderView::customContextMenuRequested, this, &ElaActionCommanderView::onCustomContextMenuRequested);

    _style = new ElaActionCommanderViewStyle(style());
    _style->setParent(this);
    setStyle(_style);
}

ElaActionCommanderView::~ElaActionCommanderView()
{
}

void ElaActionCommanderView::onCustomContextMenuRequested(const QPoint& pos)
{
    auto modelIndex = indexAt(pos);
    auto actionCommander = ElaActionCommander::getInstance();
    ElaMenu menu(this);
    menu.setMinimumWidth(200);
    menu.setMenuItemHeight(35);
    connect(menu.addElaIconAction(ElaIconType::LocationArrow, "定位到当前位置"), &QAction::triggered, this, [=]() {
        auto selectedIndexList = selectionModel()->selectedRows(0);
        if (selectedIndexList.isEmpty())
        {
            return;
        }
        auto vScrollBar = verticalScrollBar();
        int startValue = vScrollBar->value();
        scrollTo(selectedIndexList[0], QAbstractItemView::PositionAtCenter);
        int endValue = vScrollBar->value();
        QPropertyAnimation* scrollAnimation = new QPropertyAnimation(vScrollBar, "value");
        scrollAnimation->setEasingCurve(QEasingCurve::OutSine);
        scrollAnimation->setDuration(255);
        scrollAnimation->setStartValue(startValue);
        scrollAnimation->setEndValue(endValue);
        scrollAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    });
    connect(menu.addElaIconAction(ElaIconType::ListTree, "展开全部"), &QAction::triggered, this, [=]() {
        expandAll();
    });
    connect(menu.addElaIconAction(ElaIconType::ListUl, "收起全部"), &QAction::triggered, this, [=]() {
        collapseAll();
    });
    auto undoAction = menu.addElaIconAction(ElaIconType::RotateLeft, "撤销");
    undoAction->setEnabled(actionCommander->getCommanderUndoState(_pDomainName) == ElaActionCommanderType::UndoValid);
    connect(undoAction, &QAction::triggered, this, [=]() {
        actionCommander->undoCommand(_pDomainName);
    });
    auto redoAction = menu.addElaIconAction(ElaIconType::RotateRight, "重做");
    redoAction->setEnabled(actionCommander->getCommanderRedoState(_pDomainName) == ElaActionCommanderType::RedoValid);
    connect(redoAction, &QAction::triggered, this, [=]() {
        actionCommander->redoCommand(_pDomainName);
    });
    if (modelIndex.isValid())
    {
        // 如果是次要行 定位到主要行
        if (modelIndex.parent().isValid())
        {
            modelIndex = modelIndex.parent();
        }
        connect(menu.addElaIconAction(ElaIconType::ArrowsRotate, "运行到此处"), &QAction::triggered, this, [=]() {
            Q_EMIT runToCommand(_pDomainName, modelIndex.row());
        });
    }
    menu.exec(QCursor::pos());
}

void ElaActionCommanderView::paintEvent(QPaintEvent* event)
{
    QTreeView::paintEvent(event);
    // 绘制底边线
    auto viewPort = viewport();
    QPainter painter(viewPort);
    painter.save();
    painter.setPen(ElaThemeColor(_style->getThemeMode(), BasicBorder));
    QRect viewPortRect = viewPort->rect();
    painter.drawLine(viewPortRect.x(), viewPortRect.bottom(), viewPortRect.right(), viewPortRect.bottom());
    painter.restore();
}
