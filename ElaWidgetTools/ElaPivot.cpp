#include "ElaPivot.h"

#include <QPainter>
#include <QScroller>
#include <QVBoxLayout>

#include "ElaPivotModel.h"
#include "ElaPivotPrivate.h"
#include "ElaPivotStyle.h"
#include "ElaPivotView.h"
ElaPivot::ElaPivot(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaPivotPrivate())
{
    Q_D(ElaPivot);
    d->q_ptr = this;
    d->_pTextPixelSize = 20;
    setFixedHeight(40);
    setObjectName("ElaPivot");
    setStyleSheet("#ElaPivot{background-color:transparent;}");
    setMouseTracking(true);

    d->_listView = new ElaPivotView(this);
    d->_listView->setMinimumHeight(0);
    d->_listView->setFlow(QListView::LeftToRight);
    d->_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_listModel = new ElaPivotModel(this);
    d->_listView->setModel(d->_listModel);
    d->_listStyle = new ElaPivotStyle(d->_listView->style());
    d->_listView->setStyle(d->_listStyle);
    d->_listView->setPivotStyle(d->_listStyle);

    QFont textFont = this->font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
    textFont.setPixelSize(d->_pTextPixelSize);
    d->_listView->setFont(textFont);

    QScroller::grabGesture(d->_listView->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller* scroller = QScroller::scroller(d->_listView->viewport());
    QScrollerProperties properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    scroller->setScrollerProperties(properties);

    connect(scroller, &QScroller::stateChanged, this, [=](QScroller::State newstate) {
        if (newstate == QScroller::Pressed)
        {
            d->_listStyle->setPressIndex(d->_listView->indexAt(d->_listView->mapFromGlobal(QCursor::pos())));
            d->_listView->viewport()->update();
        }
        else if (newstate == QScroller::Scrolling || newstate == QScroller::Inactive)
        {
            d->_listStyle->setPressIndex(QModelIndex());
            d->_listView->viewport()->update();
        }
    });
    connect(d->_listView, &ElaPivotView::clicked, this, [=](const QModelIndex& index) {
        if (index.row() != d->_listStyle->getCurrentIndex())
        {
            Q_EMIT pCurrentIndexChanged();
        }
        d->_listView->doCurrentIndexChangedAnimation(index);
        d->_listStyle->setCurrentIndex(index.row());
        Q_EMIT pivotClicked(index.row());
    });
    connect(d->_listView, &ElaPivotView::doubleClicked, this, [=](const QModelIndex& index) {
        Q_EMIT pivotDoubleClicked(index.row());
    });
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(d->_listView);
}

ElaPivot::~ElaPivot()
{
    Q_D(ElaPivot);
    delete d->_listStyle;
}

void ElaPivot::appendPivot(QString pivotTitle)
{
    Q_D(ElaPivot);
    d->_listModel->appendPivot(pivotTitle);
    d->_checkCurrentIndex();
}

void ElaPivot::removePivot(QString pivotTitle)
{
    Q_D(ElaPivot);
    d->_listModel->removePivot(pivotTitle);
    d->_checkCurrentIndex();
}

void ElaPivot::setTextPixelSize(int textPixelSize)
{
    Q_D(ElaPivot);
    if (textPixelSize > 0)
    {
        d->_pTextPixelSize = textPixelSize;
        QFont textFont = this->font();
        textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
        textFont.setPixelSize(d->_pTextPixelSize);
        d->_listView->setFont(textFont);
        Q_EMIT pTextPixelSizeChanged();
    }
}

int ElaPivot::getTextPixelSize() const
{
    Q_D(const ElaPivot);
    return d->_pTextPixelSize;
}

void ElaPivot::setCurrentIndex(int currentIndex)
{
    Q_D(ElaPivot);
    if (currentIndex < d->_listModel->getPivotListCount())
    {
        QModelIndex index = d->_listModel->index(currentIndex, 0);
        d->_listView->doCurrentIndexChangedAnimation(index);
        if (index.row() != d->_listStyle->getCurrentIndex())
        {
            Q_EMIT pCurrentIndexChanged();
        }
        d->_listStyle->setCurrentIndex(currentIndex);
    }
}

int ElaPivot::getCurrentIndex() const
{
    Q_D(const ElaPivot);
    return d->_listView->currentIndex().row();
}

void ElaPivot::setPivotSpacing(int pivotSpacing)
{
    Q_D(ElaPivot);
    if (pivotSpacing >= 0)
    {
        d->_listStyle->setPivotSpacing(pivotSpacing);
        d->_listView->doItemsLayout();
        Q_EMIT pPivotSpacingChanged();
    }
}

int ElaPivot::getPivotSpacing() const
{
    Q_D(const ElaPivot);
    return d->_listStyle->getPivotSpacing();
}

void ElaPivot::setMarkWidth(int markWidth)
{
    Q_D(ElaPivot);
    if (markWidth >= 0)
    {
        d->_listView->setMarkWidth(markWidth);
        d->_listView->viewport()->update();
        Q_EMIT pMarkWidthChanged();
    }
}

int ElaPivot::getMarkWidth() const
{
    Q_D(const ElaPivot);
    return d->_listView->getMarkWidth();
}
