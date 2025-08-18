#ifndef ELATABLEVIEWPRIVATE_H
#define ELATABLEVIEWPRIVATE_H

#include <QObject>

#include "ElaProperty.h"

class ElaTableView;
class ElaTableViewStyle;
class ElaTableViewPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTableView)
public:
    explicit ElaTableViewPrivate(QObject* parent = nullptr);
    ~ElaTableViewPrivate();

private:
    ElaTableViewStyle* _tableViewStyle{nullptr};
};

#endif // ELATABLEVIEWPRIVATE_H
