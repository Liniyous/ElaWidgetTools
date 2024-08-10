#include "ElaScrollPage.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QScroller>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "ElaBreadcrumbBar.h"
#include "ElaNavigationRouter.h"
#include "ElaScrollArea.h"
#include "private/ElaScrollPagePrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaScrollPage, int, BorderRadius)
ElaScrollPage::ElaScrollPage(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaScrollPagePrivate())
{
    Q_D(ElaScrollPage);
    setProperty("ElaBaseClassName", "ElaScrollPage");
    d->q_ptr = this;
    d->_breadcrumbBar = new ElaBreadcrumbBar(this);
    connect(d->_breadcrumbBar, &ElaBreadcrumbBar::breadcrumbClicked, this, [=](QString breadcrumb, QStringList lastBreadcrumbList) {
        if (d->_centralWidgetMap.contains(breadcrumb))
        {
            int widgetIndex = d->_centralWidgetMap.value(breadcrumb);
            d->_switchCentralStackIndex(widgetIndex, d->_navigationTargetIndex);
            d->_navigationTargetIndex = widgetIndex;
            QVariantMap routeData = QVariantMap();
            routeData.insert("ElaScrollPageCheckSumKey", "BreadcrumbClicked");
            routeData.insert("LastBreadcrumbList", lastBreadcrumbList);
            ElaNavigationRouter::getInstance()->navigationRoute(d,"onNavigationRouteBack", routeData);
        } });
    d->_pageTitleLayout = new QHBoxLayout();
    d->_pageTitleLayout->setContentsMargins(0, 0, 0, 0);
    d->_pageTitleLayout->addWidget(d->_breadcrumbBar);

    d->_pBorderRadius = 5;
    d->_centralStackedWidget = new QStackedWidget(this);
    d->_centralStackedWidget->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(d->_pageTitleLayout);
    mainLayout->addWidget(d->_centralStackedWidget);
    setContentsMargins(20, 20, 0, 0);
}

ElaScrollPage::~ElaScrollPage()
{
}

void ElaScrollPage::addCentralWidget(QWidget* centralWidget, bool isWidgetResizeable, bool isVerticalGrabGesture, qreal mousePressEventDelay)
{
    Q_D(ElaScrollPage);
    if (!centralWidget)
    {
        return;
    }
    if (centralWidget->windowTitle().isEmpty())
    {
        centralWidget->setWindowTitle(QString("Page_%1").arg(d->_centralStackedWidget->count()));
    }
    if (d->_centralStackedWidget->count() == 0)
    {
        d->_breadcrumbBar->appendBreadcrumb(centralWidget->windowTitle());
    }
    ElaScrollArea* scrollArea = new ElaScrollArea(this);
    scrollArea->setIsAnimation(Qt::Vertical, true);
    scrollArea->setWidgetResizable(isWidgetResizeable);
    scrollArea->setIsGrabGesture(Qt::Vertical, isVerticalGrabGesture, mousePressEventDelay);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidget(centralWidget);
    centralWidget->setObjectName("ElaScrollPage_CentralPage");
    centralWidget->setStyleSheet("#ElaScrollPage_CentralPage{background-color:transparent;}");
    d->_centralWidgetMap.insert(centralWidget->windowTitle(), d->_centralStackedWidget->count());
    d->_centralStackedWidget->addWidget(scrollArea);
}

void ElaScrollPage::navigation(int widgetIndex, bool isLogRoute)
{
    Q_D(ElaScrollPage);
    if (widgetIndex >= d->_centralStackedWidget->count() || d->_navigationTargetIndex == widgetIndex)
    {
        return;
    }
    d->_switchCentralStackIndex(widgetIndex, d->_navigationTargetIndex);
    d->_navigationTargetIndex = widgetIndex;
    if (isLogRoute)
    {
        QVariantMap routeData = QVariantMap();
        routeData.insert("ElaScrollPageCheckSumKey", "Navigation");
        QStringList breadcrumbList = d->_breadcrumbBar->getBreadcrumbList();
        routeData.insert("ElaPageTitle", breadcrumbList.last());
        ElaNavigationRouter::getInstance()->navigationRoute(d, "onNavigationRouteBack", routeData);
    }
    d->_breadcrumbBar->appendBreadcrumb(d->_centralWidgetMap.key(widgetIndex));
}

void ElaScrollPage::setPageTitleSpacing(int spacing)
{
    Q_D(ElaScrollPage);
    d->_pageTitleLayout->takeAt(0);
    d->_pageTitleLayout->addSpacing(spacing);
    d->_pageTitleLayout->addWidget(d->_breadcrumbBar);
}

int ElaScrollPage::getPageTitleSpacing() const
{
    return d_ptr->_pageTitleSpacing;
}

void ElaScrollPage::setTitleVisible(bool isVisible)
{
    Q_D(ElaScrollPage);
    d->_breadcrumbBar->setVisible(isVisible);
}
