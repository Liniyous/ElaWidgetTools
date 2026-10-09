#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELARIBBONBARPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELARIBBONBARPRIVATE_H_

#include "ElaPivot.h"
#include "ElaPropertyMacro.h"

#include <QObject>

#include "ElaRibbonBarDelegate.h"
#include "ElaRibbonBarModel.h"
#include "ElaRibbonBarView.h"
#include "ElaWidgetToolsDef.h"
#include <QStackedWidget>
class ElaBaseListView;
class ElaRibbonBar;
struct ElaRibbonMVDData {
    ElaRibbonBarModel* ribbonModel{nullptr};
    ElaRibbonBarView* ribbonView{nullptr};
    ElaRibbonBarDelegate* ribbonDelegate{nullptr};
};

class ElaRibbonBarPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaRibbonBar);

public:
    explicit ElaRibbonBarPrivate(QObject* parent = nullptr);
    ~ElaRibbonBarPrivate() override;

    ElaRibbonBarModel* getOrCreateMVDData(const QString& pageName);

    Q_SLOT void onRibbonTabBarCurrentChanged(int index);
    Q_SLOT void onRibbonPivotClicked(int index);

private:
    void _switchMainStackWidget(QWidget* targetWidget);
    void _syncRibbonPivotTitles();
    void _syncRibbonTabBarTabs();
    QStackedWidget* _mainStackedWidget{nullptr};
    ElaPivot* _ribbonPivot{nullptr};
    QTabBar* _ribbonTabBar{nullptr};
    QString _currentRibbonPage{""};
    QMap<QString, ElaRibbonMVDData> _ribbonDataMap;

    static constexpr int _groupBottomMargin{13};
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELARIBBONBARPRIVATE_H_
