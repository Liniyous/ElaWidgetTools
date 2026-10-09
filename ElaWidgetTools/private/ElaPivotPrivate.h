#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPIVOTPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPIVOTPRIVATE_H_

#include <QObject>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaPivot;
class ElaPivotModel;
class ElaPivotStyle;
class ElaPivotView;
class ElaPivotPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaPivot)
    ELA_PROPERTY_D(int, TextPixelSize)
public:
    explicit ElaPivotPrivate(QObject* parent = nullptr);
    ~ElaPivotPrivate();

private:
    ElaPivotModel* _listModel{nullptr};
    ElaPivotStyle* _listStyle{nullptr};
    ElaPivotView* _listView{nullptr};
    void _checkCurrentIndex();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPIVOTPRIVATE_H_

