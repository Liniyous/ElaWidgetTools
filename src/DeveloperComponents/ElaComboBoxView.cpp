#include "ElaComboBoxView.h"

#include <QMouseEvent>

#include "private/ElaComboBoxViewPrivate.h"
ElaComboBoxView::ElaComboBoxView(QWidget* parent)
    : QListView(parent), d_ptr(new ElaComboBoxViewPrivate())
{
    Q_D(ElaComboBoxView);
    d->q_ptr = this;
}

ElaComboBoxView::~ElaComboBoxView()
{
}

void ElaComboBoxView::mousePressEvent(QMouseEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    if (index.isValid())
    {
        Q_EMIT itemPressed(index);
    }
    event->ignore();
}
