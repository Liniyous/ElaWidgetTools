#ifndef ELATABLEVIEW_H
#define ELATABLEVIEW_H

#include <QTableView>

#include "stdafx.h"
class ELA_EXPORT ElaTableView : public QTableView
{
    Q_OBJECT
public:
    explicit ElaTableView(QWidget* parent = nullptr);
    ~ElaTableView();
};

#endif // ELATABLEVIEW_H
