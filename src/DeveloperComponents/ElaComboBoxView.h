#ifndef ELACOMBOBOXVIEW_H
#define ELACOMBOBOXVIEW_H

#include <QListView>

#include "Def.h"
#include "stdafx.h"
class ElaComboBoxViewPrivate;
class ElaComboBoxView : public QListView
{
    Q_OBJECT
    Q_Q_CREATE(ElaComboBoxView)
public:
    explicit ElaComboBoxView(QWidget* parent = nullptr);
    ~ElaComboBoxView();
Q_SIGNALS:
    Q_SIGNAL void itemPressed(const QModelIndex& index);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // ELACOMBOBOXVIEW_H
