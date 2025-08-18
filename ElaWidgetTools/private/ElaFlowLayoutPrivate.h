#ifndef ELAFLOWLAYOUTPRIVATE_H
#define ELAFLOWLAYOUTPRIVATE_H

#include <QLayout>
#include <QMap>
#include <QObject>
#include <QStyle>

#include "ElaProperty.h"
class ElaFlowLayout;
class ElaFlowLayoutPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaFlowLayout)
public:
    explicit ElaFlowLayoutPrivate(QObject* parent = nullptr);
    ~ElaFlowLayoutPrivate();

private:
    int _doLayout(const QRect& rect, bool testOnly) const;
    int _smartSpacing(QStyle::PixelMetric pm) const;
    mutable QMap<ElaFlowLayout*, int> _lastHeightMap;
    mutable QMap<QLayoutItem*, QPoint> _lastGeometryMap;
    QList<QLayoutItem*> _itemList;
    bool _isAnimation{false};
    int _hSpacing;
    int _vSpacing;
};

#endif // ELAFLOWLAYOUTPRIVATE_H
