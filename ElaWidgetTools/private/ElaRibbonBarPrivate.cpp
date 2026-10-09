#include "ElaRibbonBarPrivate.h"
#include "ElaRibbonBar.h"

#include <QPropertyAnimation>
ElaRibbonBarPrivate::ElaRibbonBarPrivate(QObject* parent)
    : QObject(parent)
{
}

ElaRibbonBarPrivate::~ElaRibbonBarPrivate()
{
}

ElaRibbonBarModel* ElaRibbonBarPrivate::getOrCreateMVDData(const QString& pageName)
{
    Q_Q(ElaRibbonBar);
    ElaRibbonBarModel* ribbonModel = nullptr;
    if (!_ribbonDataMap.contains(pageName))
    {
        if (_currentRibbonPage.isEmpty())
        {
            _currentRibbonPage = pageName;
        }
        ElaRibbonMVDData mvdData;
        auto ribbonView = new ElaRibbonBarView(q);
        ribbonView->setMinimumHeight(0);
        auto ribbonDelegate = new ElaRibbonBarDelegate(q);
        ribbonDelegate->setBottomMargin(_groupBottomMargin);
        ribbonView->setItemDelegate(ribbonDelegate);
        ribbonView->setRibbonBarDelegate(ribbonDelegate);
        ribbonModel = new ElaRibbonBarModel(q);
        ribbonView->setRibbonBarModel(ribbonModel);
        ribbonView->setModel(ribbonModel);

        mvdData.ribbonModel = ribbonModel;
        mvdData.ribbonView = ribbonView;
        mvdData.ribbonDelegate = ribbonDelegate;
        _ribbonDataMap.insert(pageName, mvdData);
        _mainStackedWidget->addWidget(ribbonView);
        if (_ribbonPivot)
        {
            _ribbonPivot->appendPivot(pageName);
            if (_currentRibbonPage == pageName)
            {
                _ribbonPivot->setCurrentTitle(pageName);
            }
        }
        if (_ribbonTabBar)
        {
            _ribbonTabBar->blockSignals(true);
            _ribbonTabBar->addTab(pageName);
            _ribbonTabBar->blockSignals(false);
        }
    }
    else
    {
        ribbonModel = _ribbonDataMap[pageName].ribbonModel;
    }
    return ribbonModel;
}

void ElaRibbonBarPrivate::onRibbonTabBarCurrentChanged(int index)
{
    Q_Q(ElaRibbonBar);
    QString tabName = _ribbonTabBar->tabText(index);
    q->switchRibbonPage(tabName);
}

void ElaRibbonBarPrivate::onRibbonPivotClicked(int index)
{
    Q_Q(ElaRibbonBar);
    const auto pivotTitleList = _ribbonPivot->getPivotTitle();
    if (index >= 0 && index < pivotTitleList.count())
    {
        q->switchRibbonPage(pivotTitleList[index]);
    }
}

void ElaRibbonBarPrivate::_syncRibbonPivotTitles()
{
    if (!_ribbonPivot)
    {
        return;
    }
    _ribbonPivot->clearPivot();
    for (auto it = _ribbonDataMap.constBegin(); it != _ribbonDataMap.constEnd(); ++it)
    {
        _ribbonPivot->appendPivot(it.key());
    }
    if (!_currentRibbonPage.isEmpty())
    {
        _ribbonPivot->setCurrentTitle(_currentRibbonPage);
    }
}

void ElaRibbonBarPrivate::_syncRibbonTabBarTabs()
{
    if (!_ribbonTabBar)
    {
        return;
    }
    _ribbonTabBar->blockSignals(true);
    while (_ribbonTabBar->count() > 0)
    {
        _ribbonTabBar->removeTab(0);
    }
    for (auto it = _ribbonDataMap.constBegin(); it != _ribbonDataMap.constEnd(); ++it)
    {
        _ribbonTabBar->addTab(it.key());
    }
    if (!_currentRibbonPage.isEmpty())
    {
        for (int i = 0; i < _ribbonTabBar->count(); i++)
        {
            if (_ribbonTabBar->tabText(i) == _currentRibbonPage)
            {
                _ribbonTabBar->setCurrentIndex(i);
                break;
            }
        }
    }
    _ribbonTabBar->blockSignals(false);
}

void ElaRibbonBarPrivate::_switchMainStackWidget(QWidget* targetWidget)
{
    Q_Q(ElaRibbonBar);
    _mainStackedWidget->setCurrentWidget(targetWidget);
    QPropertyAnimation* targetWidgetAnimation = new QPropertyAnimation(targetWidget, "pos");
    targetWidgetAnimation->setEasingCurve(QEasingCurve::OutCubic);
    targetWidgetAnimation->setDuration(300);
    targetWidgetAnimation->setStartValue(QPoint(-60, 0));
    targetWidgetAnimation->setEndValue(QPoint(0, 0));
    targetWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}