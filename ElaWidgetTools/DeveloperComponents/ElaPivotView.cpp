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
    _pMarkAnimationWidth = 0;
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
    if (index.row() >= 0)
    {
        QRect newIndexRect = visualRect(index);
        QPropertyAnimation* markAnimation = new QPropertyAnimation(this, "pMarkX");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        if (_pPivotStyle->getCurrentIndex() >= 0)
        {
            markAnimation->setStartValue(_pMarkX);
            markAnimation->setEndValue(newIndexRect.center().x() - _pMarkWidth / 2);
        }
        else
        {
            markAnimation->setStartValue(newIndexRect.center().x());
            markAnimation->setEndValue(newIndexRect.center().x() - _pMarkWidth / 2);

            QPropertyAnimation* markWidthAnimation = new QPropertyAnimation(this, "pMarkAnimationWidth");
            markWidthAnimation->setDuration(300);
            markWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
            markWidthAnimation->setStartValue(0);
            markWidthAnimation->setEndValue(_pMarkWidth);
            markWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        QRect newIndexRect = visualRect(model()->index(_pPivotStyle->getCurrentIndex(), 0));
        QPropertyAnimation* markAnimation = new QPropertyAnimation(this, "pMarkX");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(_pMarkX);
        markAnimation->setEndValue(newIndexRect.center().x());
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* markWidthAnimation = new QPropertyAnimation(this, "pMarkAnimationWidth");
        markWidthAnimation->setDuration(300);
        markWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markWidthAnimation->setStartValue(_pMarkAnimationWidth);
        markWidthAnimation->setEndValue(0);
        markWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    setCurrentIndex(index);
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
    painter.drawRoundedRect(QRect(_pMarkX, viewPortRect.bottom() - 4, _pMarkAnimationWidth, 3), 3, 3);
    painter.restore();
    QListView::paintEvent(event);
}
