#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAFLOWLAYOUTPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAFLOWLAYOUTPRIVATE_H_

#include <QLayout>
#include <QMap>
#include <QObject>
#include <QStyle>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaFlowLayout;
class ElaFlowLayoutPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaFlowLayout)
public:
    explicit ElaFlowLayoutPrivate(QObject* parent = nullptr);
    ~ElaFlowLayoutPrivate() override;

private:
    int _doLayout(const QRect& rect, bool testOnly) const;
    int _smartSpacing(QStyle::PixelMetric pm) const;
    mutable QMap<QLayoutItem*, QPoint> _lastGeometryMap;
    QList<QLayoutItem*> _itemList;
    bool _isAnimation{false};
    int _hSpacing;
    int _vSpacing;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAFLOWLAYOUTPRIVATE_H_

