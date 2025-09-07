#ifndef ELATABWIDGETPRIVATE_H
#define ELATABWIDGETPRIVATE_H

#include "ElaProperty.h"
#include <QDrag>
#include <QObject>
#include <QPixmap>
class ElaTabWidget;
class ElaTabBar;
class ElaTabWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTabWidget)
public:
    explicit ElaTabWidgetPrivate(QObject* parent = nullptr);
    ~ElaTabWidgetPrivate() override;
    Q_SLOT void onTabDragCreate(QDrag* drag);
    Q_SLOT void onTabDragDrop(const QMimeData* mimeData);
    Q_SLOT void onTabCloseRequested(int index);

private:
    friend class ElaCustomTabWidget;
    ElaTabBar* _customTabBar{nullptr};
    QList<QWidget*> _allTabWidgetList;
};

#endif // ELATABWIDGETPRIVATE_H
