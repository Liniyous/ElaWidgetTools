#include "ElaTabWidget.h"

#include "ElaColorAnimation.h"
#include "ElaCustomTabWidget.h"
#include "ElaTabBar.h"
#include "ElaTabDragData.h"
#include "ElaTabDropIndicator.h"
#include "ElaTabWidgetPrivate.h"
#include "ElaTheme.h"

#include <QApplication>
#include <QHoverEvent>
#include <QPainter>
#include <QSignalBlocker>
#include <QStackedWidget>
#include <QVBoxLayout>

ELA_PROPERTY_Q_CPP(ElaTabWidget, bool, IsTabTransparent)
ELA_PROPERTY_Q_CPP(ElaTabWidget, bool, IsContainerAcceptDrops)
ELA_PROPERTY_Q_CPP(ElaTabWidget, QSize, FloatWidgetSize)

ElaTabWidget::ElaTabWidget(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaTabWidgetPrivate())
{
    Q_D(ElaTabWidget);
    d->q_ptr = this;
    d->_pIsTabTransparent = false;
    d->_pIsContainerAcceptDrops = false;
    d->_pFloatWidgetSize = {700, 500};
    d->_dragScope = this;
    setAcceptDrops(true);
    d->_tabBar = new ElaTabBar(this);
    d->_pinnedTabBar = new ElaTabBar(this);
    d->_pinnedTabBar->setIsPinned(true);
    d->_pinnedTabBar->hide();
    d->_pageStackedWidget = new QStackedWidget(this);
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(d->_pinnedTabBar);
    mainLayout->addWidget(d->_tabBar);
    mainLayout->addWidget(d->_pageStackedWidget, 1);
    for (auto* bar: {d->_tabBar, d->_pinnedTabBar})
    {
        bar->setIsDragEnabled(true);
        const auto getIndex = [d, bar](int index) {
            return index < 0 ? -1 : index + (bar == d->_tabBar ? d->_pinnedTabBar->count() : 0);
        };
        connect(bar, &QTabBar::currentChanged, this, [this, getIndex](int index) {
            setCurrentIndex(getIndex(index));
        });
        connect(bar, &QTabBar::tabBarClicked, this, [this, getIndex](int index) {
            setCurrentIndex(getIndex(index));
        });
        connect(bar, &QTabBar::tabMoved, this, [this, d, getIndex](int from, int to) {
            d->_pages.move(getIndex(from), getIndex(to));
            Q_EMIT tabMoved(getIndex(from), getIndex(to));
        });
        connect(bar, &QTabBar::tabCloseRequested, this, [this, d, bar, getIndex](int index) {
            if (bar == d->_pinnedTabBar)
            {
                setIsTabPinned(getIndex(index), false);
            }
            else
            {
                Q_EMIT tabCloseRequested(getIndex(index));
            }
        });
        bar->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(bar, &QWidget::customContextMenuRequested, this, [this, bar, getIndex](const QPoint& position) {
            const int index = bar->tabAt(position);
            if (index >= 0)
            {
                QHoverEvent leave(QEvent::HoverLeave, QPointF(), QPointF());
                QApplication::sendEvent(bar, &leave);
                setCurrentIndex(getIndex(index));
                Q_EMIT tabContextMenuRequested(getIndex(index), bar->mapToGlobal(position));
            }
        });
        connect(bar, &ElaTabBar::tabDragRequested, d, [d, getIndex](int index) {
            d->startTabDrag(getIndex(index));
        });
        connect(bar, &ElaTabBar::tabDropRequested, this, [this, d, bar, getIndex](const QMimeData* data, int index, Qt::DropAction action) {
            auto* source = getDragSource(data);
            if (source && (source == this || (getDragScope() && source->getDragScope() == getDragScope())))
            {
                transferTab(source, getDragPage(data), getIndex(index), bar == d->_pinnedTabBar, action);
            }
        });
    }
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this]() {
        update();
    });
    qApp->installEventFilter(d);
}

ElaTabWidget::~ElaTabWidget() = default;

int ElaTabWidget::addTab(QWidget* page, const QString& text)
{
    return insertTab(count(), page, {}, text);
}

int ElaTabWidget::addTab(QWidget* page, const QIcon& icon, const QString& text)
{
    return insertTab(count(), page, icon, text);
}

int ElaTabWidget::insertTab(int index, QWidget* page, const QString& text)
{
    return insertTab(index, page, {}, text);
}

int ElaTabWidget::insertTab(int index, QWidget* page, const QIcon& icon, const QString& text, bool isPinned)
{
    Q_D(ElaTabWidget);
    if (!page)
    {
        return -1;
    }
    if (indexOf(page) >= 0)
    {
        return indexOf(page);
    }
    auto* bar = isPinned ? d->_pinnedTabBar : d->_tabBar;
    const int base = isPinned ? 0 : d->_pinnedTabBar->count();
    index = qBound(base, index < 0 ? count() : index, base + bar->count());
    const QSignalBlocker blocker(bar);
    bar->insertTab(index - base, icon, text);
    d->_pages.insert(index, page);
    d->_pageStackedWidget->addWidget(page);
    if (auto* button = bar->tabButton(index - base, QTabBar::RightSide))
    {
        button->setToolTip(isPinned ? "取消固定" : "关闭");
    }
    d->updateTabRows();
    if (count() == 1)
    {
        setCurrentWidget(page);
    }
    return index;
}

void ElaTabWidget::removeTab(int index)
{
    Q_D(ElaTabWidget);
    if (index < 0 || index >= count())
    {
        return;
    }
    auto* page = d->_pages.at(index);
    auto* current = currentWidget();
    auto* bar = d->getTabBar(index);
    const QSignalBlocker blocker(bar);
    bar->removeTab(d->getBarIndex(index));
    d->_pages.removeAt(index);
    d->_pageStackedWidget->removeWidget(page);
    page->hide();
    page->setParent(this);
    d->updateTabRows();
    if (current == page)
    {
        if (count() > 0)
        {
            const QSignalBlocker currentBlocker(this);
            setCurrentIndex(qMin(index, count() - 1));
        }
        Q_EMIT currentChanged(currentIndex());
        Q_EMIT currentWidgetChanged(currentWidget());
    }
}

int ElaTabWidget::count() const
{
    Q_D(const ElaTabWidget);
    return d->_pages.size();
}

QWidget* ElaTabWidget::widget(int index) const
{
    Q_D(const ElaTabWidget);
    return d->_pages.value(index);
}

int ElaTabWidget::indexOf(QWidget* page) const
{
    Q_D(const ElaTabWidget);
    return d->_pages.indexOf(page);
}

QWidget* ElaTabWidget::currentWidget() const
{
    Q_D(const ElaTabWidget);
    return d->_pageStackedWidget->currentWidget();
}

int ElaTabWidget::currentIndex() const
{
    return indexOf(currentWidget());
}

void ElaTabWidget::setCurrentIndex(int index)
{
    if (auto* page = widget(index))
    {
        setCurrentWidget(page);
    }
}

void ElaTabWidget::setCurrentWidget(QWidget* page)
{
    Q_D(ElaTabWidget);
    const int index = indexOf(page);
    if (index < 0)
    {
        return;
    }
    auto* previous = currentWidget();
    d->_pageStackedWidget->setCurrentWidget(page);
    auto* bar = d->getTabBar(index);
    {
        const QSignalBlocker blocker(bar);
        bar->setCurrentIndex(d->getBarIndex(index));
    }
    d->_pinnedTabBar->setIsActive(bar == d->_pinnedTabBar);
    d->_tabBar->setIsActive(bar == d->_tabBar);
    bar->locateCurrentTab();
    if (previous != page || count() == 1)
    {
        Q_EMIT currentChanged(index);
        Q_EMIT currentWidgetChanged(page);
    }
}

QString ElaTabWidget::tabText(int index) const
{
    Q_D(const ElaTabWidget);
    return d->getTabBar(index)->tabText(d->getBarIndex(index));
}

void ElaTabWidget::setTabText(int index, const QString& text)
{
    Q_D(ElaTabWidget);
    d->getTabBar(index)->setTabText(d->getBarIndex(index), text);
}

QIcon ElaTabWidget::tabIcon(int index) const
{
    Q_D(const ElaTabWidget);
    return d->getTabBar(index)->tabIcon(d->getBarIndex(index));
}

void ElaTabWidget::setTabIcon(int index, const QIcon& icon)
{
    Q_D(ElaTabWidget);
    d->getTabBar(index)->setTabIcon(d->getBarIndex(index), icon);
}

QString ElaTabWidget::tabToolTip(int index) const
{
    Q_D(const ElaTabWidget);
    return d->getTabBar(index)->tabToolTip(d->getBarIndex(index));
}

void ElaTabWidget::setTabToolTip(int index, const QString& text)
{
    Q_D(ElaTabWidget);
    d->getTabBar(index)->setTabToolTip(d->getBarIndex(index), text);
}

void ElaTabWidget::setTabsClosable(bool closable)
{
    Q_D(ElaTabWidget);
    d->_tabBar->setTabsClosable(closable);
    d->_pinnedTabBar->setTabsClosable(closable);
}

void ElaTabWidget::setTabSize(QSize size)
{
    Q_D(ElaTabWidget);
    d->_tabBar->setTabSize(size);
    d->_pinnedTabBar->setTabSize(size);
    Q_EMIT pTabSizeChanged();
}

QSize ElaTabWidget::getTabSize() const
{
    Q_D(const ElaTabWidget);
    return d->_tabBar->getTabSize();
}

void ElaTabWidget::setIsTabWidthAdaptive(bool isAdaptive)
{
    Q_D(ElaTabWidget);
    if (getIsTabWidthAdaptive() == isAdaptive)
    {
        return;
    }
    d->_tabBar->setIsTabWidthAdaptive(isAdaptive);
    d->_pinnedTabBar->setIsTabWidthAdaptive(isAdaptive);
    Q_EMIT pIsTabWidthAdaptiveChanged();
}

bool ElaTabWidget::getIsTabWidthAdaptive() const
{
    Q_D(const ElaTabWidget);
    return d->_tabBar->getIsTabWidthAdaptive();
}

void ElaTabWidget::setIsTabPinned(int index, bool isPinned)
{
    if (!widget(index) || getIsTabPinned(index) == isPinned)
    {
        return;
    }
    transferTab(this, widget(index), getTabBar(true)->count(), isPinned);
    Q_EMIT tabPinnedChanged(currentIndex(), isPinned);
}

bool ElaTabWidget::getIsTabPinned(int index) const
{
    Q_D(const ElaTabWidget);
    return index >= 0 && index < d->_pinnedTabBar->count();
}

void ElaTabWidget::setIsTabDragEnabled(bool isEnabled)
{
    Q_D(ElaTabWidget);
    d->_tabBar->setIsDragEnabled(isEnabled);
    d->_pinnedTabBar->setIsDragEnabled(isEnabled);
}

void ElaTabWidget::setIsTransferManaged(bool isManaged)
{
    Q_D(ElaTabWidget);
    d->_isTransferManaged = isManaged;
}

void ElaTabWidget::setIsDropIndicatorEnabled(bool isEnabled)
{
    Q_D(ElaTabWidget);
    d->_isDropIndicatorEnabled = isEnabled;
    if (isEnabled && !d->_dropIndicator)
    {
        d->_dropIndicator = new ElaTabDropIndicator(this);
    }
    if (!isEnabled && d->_dropIndicator)
    {
        d->_dropIndicator->hideTarget(false);
    }
}

bool ElaTabWidget::getIsDropIndicatorEnabled() const
{
    Q_D(const ElaTabWidget);
    return d->_isDropIndicatorEnabled;
}

void ElaTabWidget::setIsSplitDropEnabled(bool isEnabled)
{
    Q_D(ElaTabWidget);
    d->_isSplitDropEnabled = isEnabled;
}

bool ElaTabWidget::getIsSplitDropEnabled() const
{
    Q_D(const ElaTabWidget);
    return d->_isSplitDropEnabled;
}

void ElaTabWidget::setReturnTabWidget(ElaTabWidget* group)
{
    Q_D(ElaTabWidget);
    d->_returnTabWidget = group;
}

ElaTabWidget* ElaTabWidget::getReturnTabWidget() const
{
    Q_D(const ElaTabWidget);
    return d->_returnTabWidget;
}

void ElaTabWidget::setDragScope(QObject* scope)
{
    Q_D(ElaTabWidget);
    d->_dragScope = scope;
}

QObject* ElaTabWidget::getDragScope() const
{
    Q_D(const ElaTabWidget);
    return d->_dragScope;
}

ElaTabBar* ElaTabWidget::getTabBar(bool pinned) const
{
    Q_D(const ElaTabWidget);
    return pinned ? d->_pinnedTabBar : d->_tabBar;
}

void ElaTabWidget::locateCurrentTab()
{
    Q_D(ElaTabWidget);
    d->getTabBar(currentIndex())->locateCurrentTab();
}

void ElaTabWidget::transferTab(ElaTabWidget* source, QWidget* page, int index, bool isPinned, Qt::DropAction action)
{
    Q_D(ElaTabWidget);
    if (!source || !page || source->indexOf(page) < 0)
    {
        return;
    }
    if (source != this && d->_isTransferManaged)
    {
        Q_EMIT tabTransferRequested(source, page, index, isPinned, action, ElaTabWidgetType::Center);
        return;
    }
    if (source != this && action == Qt::CopyAction)
    {
        return;
    }
    const int original = source->indexOf(page);
    const auto text = source->tabText(original);
    const auto icon = source->tabIcon(original);
    const auto tooltip = source->tabToolTip(original);
    if (source == this && original < index)
    {
        --index;
    }
    const bool sourceCurrentChanged = source != this && source->currentWidget() == page;
    {
        const QSignalBlocker sourceBlocker(source);
        const QSignalBlocker targetBlocker(this);
        source->removeTab(original);
        const int inserted = insertTab(index, page, icon, text, isPinned);
        setTabToolTip(inserted, tooltip);
        setCurrentWidget(page);
    }
    if (sourceCurrentChanged)
    {
        Q_EMIT source->currentChanged(source->currentIndex());
        Q_EMIT source->currentWidgetChanged(source->currentWidget());
    }
    Q_EMIT currentChanged(currentIndex());
    Q_EMIT currentWidgetChanged(page);
}

ElaTabWidget* ElaTabWidget::detachTab(int index, const QPoint& position)
{
    Q_D(ElaTabWidget);
    auto* page = widget(index);
    if (!page)
    {
        return nullptr;
    }
    auto* returnGroup = d->_returnTabWidget ? d->_returnTabWidget.data() : this;
    auto* window = new ElaCustomTabWidget(returnGroup);
    auto* group = window->getCustomTabWidget();
    group->setTabSize(getTabSize());
    group->setIsTabWidthAdaptive(getIsTabWidthAdaptive());
    group->setFloatWidgetSize(getFloatWidgetSize());
    group->setDragScope(getDragScope());
    group->setIsTabDragEnabled(true);
    group->setIsContainerAcceptDrops(true);
    group->setIsTransferManaged(d->_isTransferManaged);
    group->setIsDropIndicatorEnabled(d->_isDropIndicatorEnabled);
    Q_EMIT floatingGroupCreated(group);
    group->transferTab(this, page, 0, getIsTabPinned(index));
    window->resize(getFloatWidgetSize());
    window->move(position);
    window->show();
    return group;
}

ElaTabWidget* ElaTabWidget::getDragSource(const QMimeData* data)
{
    const auto* drag = qobject_cast<const ElaTabDragData*>(data);
    return drag ? drag->_source.data() : nullptr;
}

QWidget* ElaTabWidget::getDragPage(const QMimeData* data)
{
    const auto* drag = qobject_cast<const ElaTabDragData*>(data);
    return drag ? drag->_page.data() : nullptr;
}

void ElaTabWidget::paintEvent(QPaintEvent*)
{
    Q_D(ElaTabWidget);
    if (!d->_pIsTabTransparent)
    {
        QPainter painter(this);
        painter.fillRect(rect(), ElaColorAnimation::getColor(this, ElaThemeColor(eTheme->getThemeMode(), WindowBase)));
    }
}
