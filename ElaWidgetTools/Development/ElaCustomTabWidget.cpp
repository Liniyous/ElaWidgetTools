#include "ElaCustomTabWidget.h"

#include "ElaScrollBar.h"
#include "ElaTabBar.h"
#include "ElaTabWidget.h"

#include <QCloseEvent>
#include <QCursor>
#include <QTimer>
#include <QVBoxLayout>

ElaCustomTabWidget::ElaCustomTabWidget(QWidget* parent)
    : ElaCustomWidget(parent ? parent->window() : nullptr)
{
    _customTabWidget = new ElaTabWidget(this);
    _customTabWidget->setReturnTabWidget(qobject_cast<ElaTabWidget*>(parent));
    _customTabWidget->setIsTabTransparent(true);
    _mainLayout->setSpacing(0);
    _appBar->setIsIconVisible(false);
    setWindowTitle({});
    _tabBarWidget = new QWidget(this);
    auto* tabBarLayout = new QVBoxLayout(_tabBarWidget);
    tabBarLayout->setContentsMargins(6, 8, 6, 0);
    tabBarLayout->setSpacing(0);
    tabBarLayout->addWidget(_customTabWidget->getTabBar(true));
    tabBarLayout->addWidget(_customTabWidget->getTabBar());
    _appBar->setCustomWidget(ElaAppBarType::LeftArea, _tabBarWidget, this, "_getIsTitleBarDraggable");
    _tabBarWidget->setMaximumHeight(QWIDGETSIZE_MAX);
    _tabBarWidget->installEventFilter(this);
    setCentralWidget(_customTabWidget);
    connect(_customTabWidget, &ElaTabWidget::currentChanged, this, [this](int index) {
        if (index < 0)
        {
            QTimer::singleShot(0, this, [this]() {
                if (_customTabWidget->count() == 0)
                {
                    close();
                }
            });
        }
    });
    connect(_customTabWidget, &ElaTabWidget::tabCloseRequested, this, [this](int index) {
        if (auto* returnGroup = _customTabWidget->getReturnTabWidget())
        {
            returnGroup->transferTab(_customTabWidget, _customTabWidget->widget(index), returnGroup->count(), _customTabWidget->getIsTabPinned(index));
        }
        else
        {
            auto* page = _customTabWidget->widget(index);
            _customTabWidget->removeTab(index);
            page->deleteLater();
        }
    });
}

ElaTabWidget* ElaCustomTabWidget::getCustomTabWidget() const
{
    return _customTabWidget;
}

void ElaCustomTabWidget::reject()
{
    close();
}

void ElaCustomTabWidget::closeEvent(QCloseEvent* event)
{
    if (auto* returnGroup = _customTabWidget->getReturnTabWidget())
    {
        while (_customTabWidget->count() > 0)
        {
            returnGroup->transferTab(_customTabWidget, _customTabWidget->widget(0), returnGroup->count(), _customTabWidget->getIsTabPinned(0));
        }
    }
    event->accept();
}

bool ElaCustomTabWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == _tabBarWidget && event->type() == QEvent::LayoutRequest)
    {
        const int height = _tabBarWidget->layout()->sizeHint().height();
        if (_appBar->getAppBarHeight() != height)
        {
            _appBar->setAppBarHeight(height);
        }
    }
    return ElaCustomWidget::eventFilter(watched, event);
}

bool ElaCustomTabWidget::_getIsTitleBarDraggable() const
{
    const auto position = QCursor::pos();
    for (auto* bar: {_customTabWidget->getTabBar(), _customTabWidget->getTabBar(true)})
    {
        if (!bar->isVisible() || !bar->rect().contains(bar->mapFromGlobal(position)))
        {
            continue;
        }
        auto* scrollBar = bar->getScrollBar();
        return bar->tabAt(bar->mapFromGlobal(position)) < 0
            && !(scrollBar->isVisible() && scrollBar->rect().contains(scrollBar->mapFromGlobal(position)));
    }
    return true;
}
