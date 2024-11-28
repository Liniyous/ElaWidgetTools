#ifndef ELATABLEVIEW_H
#define ELATABLEVIEW_H

#include <QTableView>

#include "stdafx.h"

class ElaTableViewPrivate;
class ELA_EXPORT ElaTableView : public QTableView
{
    Q_OBJECT
    Q_Q_CREATE(ElaTableView)
    Q_PROPERTY_CREATE_Q_H(int, HeaderMargin)
public:
    explicit ElaTableView(QWidget* parent = nullptr);
    ~ElaTableView();

Q_SIGNALS:
    Q_SIGNAL void tableViewShow();
    Q_SIGNAL void tableViewHide();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
};

#endif // ELATABLEVIEW_H
