#ifndef ELATABBARPRIVATE_H
#define ELATABBARPRIVATE_H

#include <QObject>
#include <QPixmap>

#include "ElaProperty.h"
class ElaTabBar;
class ElaTabBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTabBar)
public:
    explicit ElaTabBarPrivate(QObject* parent = nullptr);
    ~ElaTabBarPrivate();

private:
    QPixmap _lastDragPix;
};

#endif // ELATABBARPRIVATE_H
