#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATABWIDGETPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATABWIDGETPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaSingletonMacro.h"
#include "ElaWidgetToolsExport.h"
#include <QDrag>
#include <QObject>
#include <QPixmap>
#include <QPointer>
class QWidget;
class ElaCustomTabWidget;
class ElaDragMonitor : public QObject
{
    Q_OBJECT
    ELA_VARIABLE(bool, IsInDrag)
    ELA_SINGLETON(ElaDragMonitor)
private:
    explicit ElaDragMonitor(QObject* parent = nullptr);
    ~ElaDragMonitor() override;
};

class ElaTabWidget;
class ElaTabBar;
class ElaTabWidgetPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaTabWidget)
    ELA_PROPERTY_D(bool, IsTabTransparent)
    ELA_PROPERTY_D(bool, IsContainerAcceptDrops);
    ELA_PROPERTY_D(QSize, TabSize)
    ELA_PROPERTY_D(QSize, FloatWidgetSize)
public:
    explicit ElaTabWidgetPrivate(QObject* parent = nullptr);
    ~ElaTabWidgetPrivate() override;
    Q_SLOT void onTabDragCreate(QMimeData* mimeData);
    Q_SLOT void onTabDragEnter(QMimeData* mimeData);
    Q_SLOT void onTabDragLeave(QMimeData* mimeData);
    Q_SLOT void onTabDragDrop(QMimeData* mimeData);
    Q_SLOT void onTabCloseRequested(int index);

private:
    friend class ElaCustomTabWidget;
    ElaTabBar* _tabBar{nullptr};
    ElaTabBar* _customTabBar{nullptr};
    // 页面拖出后仍可能被 customwidget 归还，因此跟随页面生命周期而非 tab 成员关系
    QList<QPointer<QWidget>> _allTabWidgetList;
    QList<QPointer<ElaCustomTabWidget>> _customTabWidgetList;
    void _addTabWidgetToList(QWidget* widget);
    void _removeTabWidgetFromList(QWidget* widget);
    void _registerCustomTabWidget(ElaCustomTabWidget* customTabWidget);
    void _unregisterCustomTabWidget(ElaCustomTabWidget* customTabWidget, ElaTabBar* customTabBar);
    void _clearAllTabWidgetList();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATABWIDGETPRIVATE_H_
