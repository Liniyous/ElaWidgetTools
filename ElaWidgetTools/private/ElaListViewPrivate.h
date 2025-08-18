#ifndef ELALISTVIEWPRIVATE_H
#define ELALISTVIEWPRIVATE_H

#include <QObject>

#include "ElaProperty.h"

class ElaListView;
class ElaListViewStyle;
class ElaListViewPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaListView)
public:
    explicit ElaListViewPrivate(QObject* parent = nullptr);
    ~ElaListViewPrivate();

private:
    ElaListViewStyle* _listViewStyle{nullptr};
};

#endif // ELALISTVIEWPRIVATE_H
