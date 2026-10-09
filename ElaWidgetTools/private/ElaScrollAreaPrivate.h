#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLAREAPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLAREAPRIVATE_H_

#include <QObject>
#include <QScrollBar>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaScrollBar;
class ElaScrollArea;
class ElaScrollAreaPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaScrollArea)
public:
    explicit ElaScrollAreaPrivate(QObject* parent = nullptr);
    ~ElaScrollAreaPrivate();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLAREAPRIVATE_H_

