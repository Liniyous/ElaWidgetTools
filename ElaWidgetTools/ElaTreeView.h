#ifndef ELATREEVIEW_H
#define ELATREEVIEW_H

#include <QTreeView>

#include "ElaProperty.h"

class ElaTreeViewPrivate;
class ELA_EXPORT ElaTreeView : public QTreeView
{
    Q_OBJECT
    Q_Q_CREATE(ElaTreeView)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(int, HeaderMargin)
public:
    explicit ElaTreeView(QWidget* parent = nullptr);
    ~ElaTreeView();
};

#endif // ELATREEVIEW_H
