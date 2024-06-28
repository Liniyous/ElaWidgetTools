#ifndef ELAICON_H
#define ELAICON_H
#include <QIcon>

#include "Def.h"
#include "singleton.h"
#include "stdafx.h"
class ELA_EXPORT ElaIcon
{
    Q_SINGLETON_CREATE_H(ElaIcon)
private:
    explicit ElaIcon();
    ~ElaIcon();

public:
    QIcon getElaIcon(ElaIconType awesome);
    QIcon getElaIcon(ElaIconType awesome, QColor iconColor);
    QIcon getElaIcon(ElaIconType awesome, int pixelSize);
    QIcon getElaIcon(ElaIconType awesome, int pixelSize, QColor iconColor);
    QIcon getElaIcon(ElaIconType awesome, int pixelSize, int fixedWidth, int fixedHeight);
    QIcon getElaIcon(ElaIconType awesome, int pixelSize, int fixedWidth, int fixedHeight, QColor iconColor);
};

#endif // ELAICON_H
