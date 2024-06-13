#ifndef ELALISTVIEW_H
#define ELALISTVIEW_H

#include <QListView>

#include "stdafx.h"
class ELA_EXPORT ElaListView : public QListView
{
    Q_OBJECT
public:
    explicit ElaListView(QWidget* parent = nullptr);
    ~ElaListView();
};

#endif // ELALISTVIEW_H
