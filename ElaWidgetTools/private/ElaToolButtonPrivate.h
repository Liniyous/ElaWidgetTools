#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOOLBUTTONPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOOLBUTTONPRIVATE_H_

#include <QObject>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaToolButton;
class ElaToolButtonStyle;
class ElaToolButtonPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaToolButton)
public:
    explicit ElaToolButtonPrivate(QObject* parent = nullptr);
    ~ElaToolButtonPrivate();

private:
    ElaToolButtonStyle* _toolButtonStyle{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOOLBUTTONPRIVATE_H_

