#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAICON_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAICON_H_
#include <QIcon>

#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
#include "ElaSingletonMacro.h"
class ELA_EXPORT ElaIcon
{
    ELA_SINGLETON_H(ElaIcon)
private:
    explicit ElaIcon();
    ~ElaIcon();

public:
    QIcon getElaIcon(ElaIconType::IconName awesome);
    QIcon getElaIcon(ElaIconType::IconName awesome, QColor iconColor);
    QIcon getElaIcon(ElaIconType::IconName awesome, int pixelSize);
    QIcon getElaIcon(ElaIconType::IconName awesome, int pixelSize, QColor iconColor);
    QIcon getElaIcon(ElaIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight);
    QIcon getElaIcon(ElaIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QColor iconColor);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAICON_H_
