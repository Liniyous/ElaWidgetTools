#ifndef ELATABBARPRIVATE_H
#define ELATABBARPRIVATE_H

#include "ElaProperty.h"
#include <QMimeData>
#include <QObject>
#include <QPixmap>
class ElaTabBar;
class ElaTabBarStyle;
class QTabBarPrivate;
class ElaTabBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTabBar)
public:
    explicit ElaTabBarPrivate(QObject* parent = nullptr);
    ~ElaTabBarPrivate() override;

private:
    QMimeData* _mimeData{nullptr};
    ElaTabBarStyle* _style{nullptr};
    QTabBarPrivate* _tabBarPrivate{nullptr};
};

#endif // ELATABBARPRIVATE_H
