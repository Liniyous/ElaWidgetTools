#ifndef ELALISTVIEW_H
#define ELALISTVIEW_H

#include <QListView>

#include "stdafx.h"

class ElaListViewPrivate;
class ELA_EXPORT ElaListView : public QListView
{
    Q_OBJECT
    Q_Q_CREATE(ElaListView)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)
public:
    explicit ElaListView(QWidget* parent = nullptr);
    ~ElaListView();
};

#endif // ELALISTVIEW_H
