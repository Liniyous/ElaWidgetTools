#ifndef ELATABWIDGETPRIVATE_H
#define ELATABWIDGETPRIVATE_H

#include <QDrag>
#include <QObject>
#include <QPixmap>

#include "ElaProperty.h"
class ElaTabWidget;
class ElaTabBar;
class ElaTabWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTabWidget)
public:
    explicit ElaTabWidgetPrivate(QObject* parent = nullptr);
    ~ElaTabWidgetPrivate();
    Q_SLOT void onTabBarPress(int index);
    Q_SLOT void onTabDragCreate(QDrag* drag);
    Q_SLOT void onTabDragDrop(const QMimeData* mimeData);
    Q_SLOT void onTabCloseRequested(int index);

private:
    ElaTabBar* _customTabBar{nullptr};
};

#endif // ELATABWIDGETPRIVATE_H
