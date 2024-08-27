#include "ElaPivotView.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>

#include "ElaPivotStyle.h"
#include "ElaScrollBar.h"
ElaPivotView::ElaPivotView(QWidget* parent)
    : QListView(parent)
{
    _pMarkX = 0;
    _pMarkWidth = 40;
    setObjectName("ElaPivotView");
    setStyleSheet("#ElaPivotView{background-color:transparent;}");
    setMouseTracking(true);
    setVerticalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollBar(new ElaScrollBar(this));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

ElaPivotView::~ElaPivotView()
{
}

void ElaPivotView::doCurrentIndexChangedAnimation(const QModelIndex& index)
{
    QRect newIndexRect = visualRect(index);
    QPropertyAnimation* markAnimation = new QPropertyAnimation(this, "pMarkX");
    connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
        update();
    });
    markAnimation->setDuration(300);
    markAnimation->setEasingCurve(QEasingCurve::InOutSine);
    markAnimation->setStartValue(_pMarkX);
    markAnimation->setEndValue(newIndexRect.center().x() - _pMarkWidth / 2);
    markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaPivotView::mouseDoubleClickEvent(QMouseEvent* event)
{
    _pPivotStyle->setPressIndex(indexAt(event->pos()));
    viewport()->update();
    QListView::mouseDoubleClickEvent(event);
}

void ElaPivotView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _pPivotStyle->setPressIndex(QModelIndex());
        viewport()->update();
    }
    QListView::mouseReleaseEvent(event);
}

void ElaPivotView::paintEvent(QPaintEvent* event)
{
    QPainter painter(viewport());
    QRect viewPortRect = viewport()->rect();
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(_pPivotStyle->getMarkColor());
    QModelIndex index = model()->index(_pPivotStyle->getCurrentIndex(), 0);
    if (index.isValid())
    {
        painter.drawRoundedRect(QRect(_pMarkX, viewPortRect.bottom() - 4, _pMarkWidth, 3), 3, 3);
    }
    painter.restore();
    QListView::paintEvent(event);
}
