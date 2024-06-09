#ifndef ELATABLEVIEW_H
#define ELATABLEVIEW_H

#include <QTableView>

#include "stdafx.h"
class ElaTableView : public QTableView
{
    Q_OBJECT
public:
    explicit ElaTableView(QWidget* parent = nullptr);
    ~ElaTableView();

private:
};

#endif // ELATABLEVIEW_H
