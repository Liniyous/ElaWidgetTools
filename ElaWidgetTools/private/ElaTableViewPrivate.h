#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATABLEVIEWPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATABLEVIEWPRIVATE_H_

#include <QObject>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaTableView;
class ElaTableViewStyle;
class ElaTableViewPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaTableView)
public:
    explicit ElaTableViewPrivate(QObject* parent = nullptr);
    ~ElaTableViewPrivate();

private:
    ElaTableViewStyle* _tableViewStyle{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATABLEVIEWPRIVATE_H_

