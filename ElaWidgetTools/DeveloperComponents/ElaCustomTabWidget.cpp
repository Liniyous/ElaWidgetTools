#include "ElaCustomTabWidget.h"

#include "ElaAppBar.h"
#include "ElaTabBar.h"
#include "ElaTabWidget.h"
#include "ElaTabWidgetPrivate.h"
#include <QDebug>
#include <QEvent>
#include <QMimeData>
#include <QMouseEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <QVariant>
ElaCustomTabWidget::ElaCustomTabWidget(QWidget* parent)
    : ElaCustomWidget(parent)
{
    _pIsFinished = false;
    resize(700, 500);
    setWindowTitle("");
#ifndef Q_OS_WIN
    setAttribute(Qt::WA_Hover);
#endif
    setMouseTracking(true);
    setWindowIcon(QIcon());
    _customTabWidget = new ElaTabWidget(this);
    _customTabWidget->setIsTabTransparent(true);
    _customTabWidget->setObjectName("ElaCustomTabWidget");
    QTabBar* originTabBar = _customTabWidget->tabBar();
    originTabBar->hide();
    setAcceptDrops(true);
    _customTabBar = new ElaTabBar(this);
    _customTabBar->setObjectName("ElaCustomTabBar");
    connect(_customTabBar, &ElaTabBar::tabMoved, this, [=](int from, int to) {
        _customTabWidget->tabBar()->moveTab(from, to);
    });
    connect(_customTabBar, &ElaTabBar::currentChanged, this, [=](int index) {
        _customTabWidget->setCurrentIndex(index);
    });
    connect(_customTabWidget, &ElaTabWidget::currentChanged, this, [=](int index) {
        if (index == -1)
        {
            _pIsFinished = true;
            hide();
        }
    });
    connect(_customTabBar, &ElaTabBar::tabCloseRequested, originTabBar, &QTabBar::tabCloseRequested);

    _customTabWidget->d_ptr->_customTabBar = _customTabBar;
    connect(_customTabBar, &ElaTabBar::tabDragCreate, _customTabWidget->d_func(), &ElaTabWidgetPrivate::onTabDragCreate);
    connect(_customTabBar, &ElaTabBar::tabDragDrop, _customTabWidget->d_func(), &ElaTabWidgetPrivate::onTabDragDrop);
    connect(_customTabBar, &ElaTabBar::tabDragEnter, _customTabWidget->d_func(), &ElaTabWidgetPrivate::onTabDragEnter);
    connect(_customTabBar, &ElaTabBar::tabDragLeave, _customTabWidget->d_func(), &ElaTabWidgetPrivate::onTabDragLeave);
    QWidget* customWidget = new QWidget(this);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(10, 0, 10, 0);
    customLayout->addStretch();
    customLayout->addWidget(_customTabBar);
    _appBar->setCustomWidget(ElaAppBarType::LeftArea, customWidget, this, "processHitTest");
    setCentralWidget(_customTabWidget);
}

ElaCustomTabWidget::~ElaCustomTabWidget()
{
    while (_customTabWidget->count() > 0)
    {
        QWidget* closeWidget = _customTabWidget->widget(0);
        ElaTabWidget* originTabWidget = closeWidget->property("ElaOriginTabWidget").value<ElaTabWidget*>();
        if (originTabWidget)
        {
            closeWidget->setProperty("CurrentCustomBar", QVariant::fromValue<ElaTabBar*>(nullptr));
            originTabWidget->addTab(closeWidget, _customTabWidget->tabIcon(0), _customTabWidget->tabText(0));
            originTabWidget->setCurrentWidget(closeWidget);
        }
        else
        {
            _customTabWidget->removeTab(0);
        }
    }
}

void ElaCustomTabWidget::addTab(QWidget* widget, QIcon tabIcon, const QString& tabTitle)
{
    _customTabBar->addTab(tabIcon, tabTitle);
    _customTabWidget->addTab(widget, tabIcon, tabTitle);
}

ElaTabBar* ElaCustomTabWidget::getCustomTabBar() const
{
    return _customTabBar;
}

ElaTabWidget* ElaCustomTabWidget::getCustomTabWidget() const
{
    return _customTabWidget;
}

bool ElaCustomTabWidget::processHitTest()
{
    auto point = _customTabBar->mapFromGlobal(QCursor::pos());
    return _customTabBar->tabAt(point) < 0;
}
