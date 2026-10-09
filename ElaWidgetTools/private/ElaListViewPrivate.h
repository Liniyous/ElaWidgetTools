#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALISTVIEWPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALISTVIEWPRIVATE_H_

#include <QObject>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaListView;
class ElaListViewStyle;
class ElaListViewPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaListView)
public:
    explicit ElaListViewPrivate(QObject* parent = nullptr);
    ~ElaListViewPrivate();

private:
    ElaListViewStyle* _listViewStyle{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALISTVIEWPRIVATE_H_

