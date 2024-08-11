#include "ElaComboBoxView.h"

#include <QMouseEvent>

ElaComboBoxView::ElaComboBoxView(QWidget* parent)
    : QListView(parent)
{
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
