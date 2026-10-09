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
    _pMarkWidth = 30;
    _pMarkAnimationWidth = 0;
    _pCurrentIndexRect = QRect();
    _pIsAnimationFinished = true;
    _markIndex = QModelIndex();
    setObjectName("ElaPivotView");
    setStyleSheet("#ElaPivotView{background-color:transparent;}");
    setMouseTracking(true);
    setVerticalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

ElaPivotView::~ElaPivotView()
{
}

bool ElaPivotView::doCurrentIndexChangedAnimation(const QModelIndex& index)
{
    const QModelIndex previousIndex = _markIndex;
    // 当前项相同时仅当mark已显示才跳过 初始化时currentIndex自动选中但mark宽度仍为0
    if (previousIndex == index && previousIndex.isValid() && _pMarkAnimationWidth > 0)
    {
        return false;
    }
    const QRect previousIndexRect = _pCurrentIndexRect;
    qreal markXStart = 0;
    qreal markXEnd = 0;
    int markWidthEnd = 0;
    if (index.row() >= 0)
    {
        _pCurrentIndexRect = visualRect(index);
        markXEnd = _pCurrentIndexRect.center().x() - _pMarkWidth / 2;
        if (previousIndex.isValid())
        {
            // 以mark上一帧记录的矩形为基准补偿布局位移 静止状态offset为0 不会叠加目标位移
            markXStart = _pMarkX + visualRect(previousIndex).center().x() - previousIndexRect.center().x();
        }
        else
        {
            markXStart = _pCurrentIndexRect.center().x();
        }
        markWidthEnd = _pMarkWidth;
    }
    else
    {
        if (!previousIndex.isValid())
        {
            return false;
        }
        _pCurrentIndexRect = visualRect(previousIndex);
        markXStart = _pCurrentIndexRect.center().x() - _pMarkWidth / 2;
        markXEnd = _pCurrentIndexRect.center().x();
        markWidthEnd = 0;
    }
    _pIsAnimationFinished = false;
    _markIndex = index;
    setCurrentIndex(index);
    _startMarkAnimation(markXStart, markXEnd, _pMarkAnimationWidth, markWidthEnd);
    return true;
}

void ElaPivotView::_startMarkAnimation(qreal markXStart, qreal markXEnd, int markWidthStart, int markWidthEnd)
{
    auto markAnimation = new QPropertyAnimation(this, "pMarkX");
    connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
        update();
    });
    connect(markAnimation, &QPropertyAnimation::finished, this, [=]() {
        _pIsAnimationFinished = true;
        update();
    });
    markAnimation->setDuration(200);
    markAnimation->setEasingCurve(QEasingCurve::InOutSine);
    markAnimation->setStartValue(markXStart);
    markAnimation->setEndValue(markXEnd);
    markAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    auto markWidthAnimation = new QPropertyAnimation(this, "pMarkAnimationWidth");
    connect(markWidthAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
        update();
    });
    markWidthAnimation->setDuration(200);
    markWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
    markWidthAnimation->setStartValue(markWidthStart);
    markWidthAnimation->setEndValue(markWidthEnd);
    markWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
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

void ElaPivotView::wheelEvent(QWheelEvent* event)
{
    QListView::wheelEvent(event);
    event->accept();
}

void ElaPivotView::paintEvent(QPaintEvent* event)
{
    QPainter painter(viewport());
    QRect viewPortRect = viewport()->rect();
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(_pPivotStyle->getMarkColor());
    QRect currentIndexRect = visualRect(_markIndex);
    const qreal markX = _pIsAnimationFinished ? currentIndexRect.center().x() - _pMarkWidth / 2
                                              : _pMarkX + currentIndexRect.x() - _pCurrentIndexRect.x();
    painter.drawRoundedRect(QRectF(markX, viewPortRect.bottom() - 4, _pMarkAnimationWidth, 3), 3, 3);
    painter.restore();
    QListView::paintEvent(event);
}
