#include "ElaRibbonBar.h"

#include "ElaRibbonBarPrivate.h"
#include "ElaText.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QPainter>
#include <QTimer>

#include <QVBoxLayout>
ElaRibbonBar::ElaRibbonBar(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaRibbonBarPrivate())
{
    Q_D(ElaRibbonBar);
    d->q_ptr = this;
    setFixedHeight(75);
    setObjectName("ElaRibbonBar");
    setStyleSheet("#ElaRibbonBar{background-color:transparent;}");
    setMouseTracking(true);

    window()->installEventFilter(this);

    d->_mainStackedWidget = new QStackedWidget(this);
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(d->_mainStackedWidget);
}

ElaRibbonBar::~ElaRibbonBar()
{
}

void ElaRibbonBar::setRibbonTabBar(QTabBar* tabBar)
{
    Q_D(ElaRibbonBar);
    if (d->_ribbonTabBar)
    {
        disconnect(d->_ribbonTabBar, &QTabBar::currentChanged, d, &ElaRibbonBarPrivate::onRibbonTabBarCurrentChanged);
    }
    d->_ribbonTabBar = tabBar;
    if (d->_ribbonTabBar)
    {
        d->_ribbonTabBar->setMovable(false);
        d->_ribbonTabBar->setTabsClosable(false);
        connect(d->_ribbonTabBar, &QTabBar::currentChanged, d, &ElaRibbonBarPrivate::onRibbonTabBarCurrentChanged);
        d->_syncRibbonTabBarTabs();
    }
}

QTabBar* ElaRibbonBar::getRibbonTabBar() const
{
    Q_D(const ElaRibbonBar);
    return d->_ribbonTabBar;
}

void ElaRibbonBar::setRibbonPivot(ElaPivot* pivot)
{
    Q_D(ElaRibbonBar);
    if (d->_ribbonPivot)
    {
        disconnect(d->_ribbonPivot, &ElaPivot::pivotClicked, d, &ElaRibbonBarPrivate::onRibbonPivotClicked);
    }
    d->_ribbonPivot = pivot;
    if (d->_ribbonPivot)
    {
        connect(d->_ribbonPivot, &ElaPivot::pivotClicked, d, &ElaRibbonBarPrivate::onRibbonPivotClicked);
        d->_syncRibbonPivotTitles();
    }
}

ElaPivot* ElaRibbonBar::getRibbonPivot() const
{
    Q_D(const ElaRibbonBar);
    return d->_ribbonPivot;
}

void ElaRibbonBar::switchRibbonPage(const QString& pageName)
{
    Q_D(ElaRibbonBar);
    if (!d->_ribbonDataMap.contains(pageName))
    {
        d->_currentRibbonPage = "";
        return;
    }
    if (d->_currentRibbonPage == pageName)
    {
        return;
    }
    d->_currentRibbonPage = pageName;
    d->_switchMainStackWidget(d->_ribbonDataMap[pageName].ribbonView);
    if (d->_ribbonPivot)
    {
        d->_ribbonPivot->setCurrentTitle(pageName);
    }
    if (d->_ribbonTabBar)
    {
        for (int i = 0; i < d->_ribbonTabBar->count(); i++)
        {
            if (pageName == d->_ribbonTabBar->tabText(i))
            {
                d->_ribbonTabBar->blockSignals(true);
                d->_ribbonTabBar->setCurrentIndex(i);
                d->_ribbonTabBar->blockSignals(false);
                break;
            }
        }
    }
    Q_EMIT ribbonPageChanged(pageName);
}

const QString& ElaRibbonBar::getCurrentRibbonPage() const
{
    Q_D(const ElaRibbonBar);
    return d->_currentRibbonPage;
}

void ElaRibbonBar::setRibbonPageEnable(const QString& pageName, bool isEnable)
{
    Q_D(ElaRibbonBar);
    if (!d->_ribbonDataMap.contains(pageName))
    {
        return;
    }
    d->_ribbonDataMap[pageName].ribbonView->setEnabled(isEnable);
}

bool ElaRibbonBar::getRibbonPageEnable(const QString& pageName) const
{
    Q_D(const ElaRibbonBar);
    if (!d->_ribbonDataMap.contains(pageName))
    {
        return false;
    }
    return d->_ribbonDataMap[pageName].ribbonView->isEnabled();
}

QAction* ElaRibbonBar::addRibbonAction(const QString& pageName, const QString& actionName)
{
    Q_D(ElaRibbonBar);
    ElaRibbonBarModel* ribbonModel = d->getOrCreateMVDData(pageName);
    auto ribbonAction = ribbonModel->addRibbonAction(actionName);
    ribbonAction->setProperty("ElaRibbonPageName", pageName);
    return ribbonAction;
}

QAction* ElaRibbonBar::addRibbonAction(const QString& pageName, const QString& actionName, ElaIconType::IconName icon)
{
    Q_D(ElaRibbonBar);
    ElaRibbonBarModel* ribbonModel = d->getOrCreateMVDData(pageName);
    auto ribbonAction = ribbonModel->addRibbonAction(actionName, icon);
    ribbonAction->setProperty("ElaRibbonPageName", pageName);
    return ribbonAction;
}

QAction* ElaRibbonBar::addRibbonAction(const QString& pageName, const QString& actionName, const QIcon& icon)
{
    Q_D(ElaRibbonBar);
    ElaRibbonBarModel* ribbonModel = d->getOrCreateMVDData(pageName);
    auto ribbonAction = ribbonModel->addRibbonAction(actionName, icon);
    ribbonAction->setProperty("ElaRibbonPageName", pageName);
    return ribbonAction;
}

QAction* ElaRibbonBar::addRibbonAction(const QString& pageName, const QString& groupName, const QString& actionName)
{
    Q_D(ElaRibbonBar);
    ElaRibbonBarModel* ribbonModel = d->getOrCreateMVDData(pageName);
    auto ribbonAction = ribbonModel->addRibbonAction(groupName, actionName);
    ribbonAction->setProperty("ElaRibbonPageName", pageName);
    return ribbonAction;
}

QAction* ElaRibbonBar::addRibbonAction(const QString& pageName, const QString& groupName, const QString& actionName, ElaIconType::IconName icon)
{
    Q_D(ElaRibbonBar);
    ElaRibbonBarModel* ribbonModel = d->getOrCreateMVDData(pageName);
    auto ribbonAction = ribbonModel->addRibbonAction(groupName, actionName, icon);
    ribbonAction->setProperty("ElaRibbonPageName", pageName);
    return ribbonAction;
}

QAction* ElaRibbonBar::addRibbonAction(const QString& pageName, const QString& groupName, const QString& actionName, const QIcon& icon)
{
    Q_D(ElaRibbonBar);
    ElaRibbonBarModel* ribbonModel = d->getOrCreateMVDData(pageName);
    auto ribbonAction = ribbonModel->addRibbonAction(groupName, actionName, icon);
    ribbonAction->setProperty("ElaRibbonPageName", pageName);
    return ribbonAction;
}

QAction* ElaRibbonBar::addRibbonWidget(const QString& pageName, const QString& groupName, QWidget* widget)
{
    Q_D(ElaRibbonBar);
    ElaRibbonBarModel* ribbonModel = d->getOrCreateMVDData(pageName);
    auto ribbonAction = ribbonModel->addRibbonAction(groupName, "ElaRibbonWidgetAction");
    ribbonAction->setProperty("ElaRibbonPageName", pageName);
    ribbonAction->setProperty("ElaRibbonWidget", QVariant::fromValue(widget));
    d->_ribbonDataMap[pageName].ribbonView->setIndexWidget(ribbonModel->getRibbonActionModelIndex(ribbonAction), widget);
    return ribbonAction;
}

void ElaRibbonBar::removeRibbonAction(QAction* action)
{
    Q_D(ElaRibbonBar);
    if (!action)
    {
        return;
    }
    auto ribbonPageName = action->property("ElaRibbonPageName").toString();
    if (!d->_ribbonDataMap.contains(ribbonPageName))
    {
        return;
    }
    auto ribbonModel = d->_ribbonDataMap[ribbonPageName].ribbonModel;
    ribbonModel->removeRibbonAction(action);
}

void ElaRibbonBar::removeRibbonAction(const QString& pageName, const QString& groupName, const QString& actionName)
{
    Q_D(ElaRibbonBar);
    if (!d->_ribbonDataMap.contains(pageName))
    {
        return;
    }
    auto ribbonModel = d->_ribbonDataMap[pageName].ribbonModel;
    ribbonModel->removeRibbonAction(groupName, actionName);
}

QList<QAction*> ElaRibbonBar::getRibbonActions(const QString& pageName)
{
    Q_D(ElaRibbonBar);
    if (!d->_ribbonDataMap.contains(pageName))
    {
        return {};
    }
    auto ribbonModel = d->_ribbonDataMap[pageName].ribbonModel;
    return ribbonModel->getRibbonActions();
}

QList<QAction*> ElaRibbonBar::getRibbonActions(const QString& pageName, const QString& groupName)
{
    Q_D(ElaRibbonBar);
    if (!d->_ribbonDataMap.contains(pageName))
    {
        return {};
    }
    auto ribbonModel = d->_ribbonDataMap[pageName].ribbonModel;
    return ribbonModel->getRibbonActions(groupName);
}

QList<QAction*> ElaRibbonBar::getRibbonActions(const QString& pageName, const QString& groupName, const QString& actionName)
{
    Q_D(ElaRibbonBar);
    if (!d->_ribbonDataMap.contains(pageName))
    {
        return {};
    }
    auto ribbonModel = d->_ribbonDataMap[pageName].ribbonModel;
    return ribbonModel->getRibbonActions(groupName, actionName);
}

int ElaRibbonBar::getGroupBottomMargin() const
{
    Q_D(const ElaRibbonBar);
    return d->_groupBottomMargin;
}

bool ElaRibbonBar::eventFilter(QObject* obj, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Resize:
    {
        QSize size = parentWidget()->size();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
        if (::IsZoomed((HWND)d->_currentWinID))
        {
            this->resize(size.width() - 14, this->height());
        }
        else
        {
            this->resize(size.width(), this->height());
        }
#else
        this->resize(size.width(), this->height());
#endif
        break;
    }
    default:
    {
        break;
    }
    }
    return QWidget::eventFilter(obj, event);
}
