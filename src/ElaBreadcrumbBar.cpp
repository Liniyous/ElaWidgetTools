#include "ElaBreadcrumbBar.h"

#include <QFontMetrics>
#include <QHeaderView>
#include <QPainter>
#include <QScroller>
#include <QVBoxLayout>

#include "DeveloperComponents/ElaBreadcrumbBarDelegate.h"
#include "DeveloperComponents/ElaBreadcrumbBarModel.h"
#include "ElaListView.h"
ElaBreadcrumbBar::ElaBreadcrumbBar(QWidget* parent)
    : QWidget{parent}
{
    setFixedHeight(37);
    setObjectName("ElaBreadcrumbBar");
    setStyleSheet("#ElaBreadcrumbBar{background-color:transparent;}");
    setMouseTracking(true);

    _listView = new ElaListView(this);
    _listView->setMinimumHeight(0);
    _listView->setFlow(QListView::LeftToRight);
    _listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _listModel = new ElaBreadcrumbBarModel(this);
    _listView->setModel(_listModel);
    _listDelegate = new ElaBreadcrumbBarDelegate(this);
    _listView->setItemDelegate(_listDelegate);
    connect(_listView, &QListView::clicked, this, [=](const QModelIndex& index) {
        if (_listModel->getBreadcrumbListCount() != 1 && index.row() != _listModel->getBreadcrumbListCount() * 2 - 2 && index.data(Qt::DisplayRole).toString() != ">")
        {
            Q_EMIT breadcrumbClicked(index.data(Qt::DisplayRole).toString(), _listModel->getBreadcrumbList());
            _listModel->removeBreadcrumb(index.row() / 2 + 1);
        } });
    QFont textFont = this->font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
    textFont.setPixelSize(28);
    _listView->setFont(textFont);

    QScroller::grabGesture(_listView->viewport(), QScroller::LeftMouseButtonGesture);
    QScrollerProperties properties = QScroller::scroller(_listView->viewport())->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    QScroller::scroller(_listView->viewport())->setScrollerProperties(properties);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(_listView);
}

ElaBreadcrumbBar::~ElaBreadcrumbBar()
{
}

void ElaBreadcrumbBar::setBreadcrumbList(QStringList breadcrumbList)
{
    _listModel->setBreadcrumbList(breadcrumbList);
}

QStringList ElaBreadcrumbBar::appendBreadcrumb(QString breadcrumb)
{
    _listModel->appendBreadcrumb(breadcrumb);
    return _listModel->getBreadcrumbList();
}

QStringList ElaBreadcrumbBar::removeBreadcrumb(QString breadcrumb)
{
    _listModel->removeBreadcrumb(breadcrumb);
    return _listModel->getBreadcrumbList();
}

int ElaBreadcrumbBar::getBreadcrumbListCount() const
{
    return _listModel->getBreadcrumbListCount();
}

QStringList ElaBreadcrumbBar::getBreadcrumbList() const
{
    return _listModel->getBreadcrumbList();
}
