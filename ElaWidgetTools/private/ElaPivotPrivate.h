#ifndef ELAPIVOTPRIVATE_H
#define ELAPIVOTPRIVATE_H

#include <QObject>

#include "ElaProperty.h"

class ElaPivot;
class ElaPivotModel;
class ElaPivotStyle;
class ElaPivotView;
class ElaPivotPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaPivot)
    Q_PROPERTY_CREATE_D(int, TextPixelSize)
public:
    explicit ElaPivotPrivate(QObject* parent = nullptr);
    ~ElaPivotPrivate();

private:
    ElaPivotModel* _listModel{nullptr};
    ElaPivotStyle* _listStyle{nullptr};
    ElaPivotView* _listView{nullptr};
    void _checkCurrentIndex();
};

#endif // ELAPIVOTPRIVATE_H
