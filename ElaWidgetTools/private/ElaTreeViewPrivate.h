#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATREEVIEWPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATREEVIEWPRIVATE_H_

#include <QObject>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaTreeView;
class ElaTreeViewStyle;
class ElaTreeViewPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaTreeView)
public:
    explicit ElaTreeViewPrivate(QObject* parent = nullptr);
    ~ElaTreeViewPrivate();

private:
    ElaTreeViewStyle* _treeViewStyle{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATREEVIEWPRIVATE_H_

