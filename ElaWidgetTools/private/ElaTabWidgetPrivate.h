#ifndef ELATABWIDGETPRIVATE_H
#define ELATABWIDGETPRIVATE_H

#include "ElaProperty.h"
#include "ElaSingleton.h"
#include <QDrag>
#include <QObject>
#include <QPixmap>
class ElaDragMonitor : public QObject
{
    Q_OBJECT
    Q_PRIVATE_CREATE(bool, IsInDrag)
    Q_SINGLETON_CREATE(ElaDragMonitor)
private:
    explicit ElaDragMonitor(QObject* parent = nullptr);
    ~ElaDragMonitor() override;
};

class ElaTabWidget;
class ElaTabBar;
class ElaTabWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTabWidget)
    Q_PROPERTY_CREATE_D(bool, IsTabTransparent)
    Q_PROPERTY_CREATE_D(bool, IsContainerAcceptDrops);
    Q_PROPERTY_CREATE_D(QSize, TabSize)
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
    QList<QWidget*> _allTabWidgetList;
    void _clearAllTabWidgetList();
};

#endif // ELATABWIDGETPRIVATE_H
