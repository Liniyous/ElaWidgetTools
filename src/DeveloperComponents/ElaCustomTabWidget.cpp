#include "ElaCustomTabWidget.h"

#include <QVBoxLayout>

#include "ElaAppBar.h"
#include "ElaTabBar.h"
#include "ElaTabWidget.h"
#include "ElaTabWidgetPrivate.h"
ElaCustomTabWidget::ElaCustomTabWidget(QWidget* parent)
    : ElaCustomWidget(parent)
{
    resize(700, 500);
    setWindowTitle("");
    setWindowIcon(QIcon());
    _customTabWidget = new ElaTabWidget(this);
    QTabBar* originTabBar = _customTabWidget->tabBar();
    originTabBar->hide();
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
            close();
        }
    });

    QWidget* customWidget = new QWidget(this);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(10, 0, 10, 0);
    customLayout->addStretch();
    customLayout->addWidget(_customTabBar);
    _appBar->setCustomWidget(ElaAppBarType::LeftArea, customWidget);
    setCentralWidget(_customTabWidget);
}

ElaCustomTabWidget::~ElaCustomTabWidget()
{
}

void ElaCustomTabWidget::addTab(QWidget* widget, QIcon& tabIcon, const QString& tabTitle)
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
