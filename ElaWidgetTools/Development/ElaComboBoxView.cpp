#include "ElaComboBoxView.h"

#include <QKeyEvent>
#include <QMouseEvent>

ElaComboBoxView::ElaComboBoxView(QWidget* parent)
    : QListView(parent)
{
}

void ElaComboBoxView::setModel(QAbstractItemModel* model)
{
    QListView::setModel(model);
    Q_EMIT modelChanged();
}

void ElaComboBoxView::mousePressEvent(QMouseEvent* event)
{
    _pressedIndex = event->button() == Qt::LeftButton ? indexAt(event->pos()) : QModelIndex();
    QListView::mousePressEvent(event);
}

void ElaComboBoxView::mouseReleaseEvent(QMouseEvent* event)
{
    const QModelIndex index = indexAt(event->pos());
    if (event->button() == Qt::LeftButton && index.isValid() && index == _pressedIndex)
    {
        Q_EMIT itemPressed(index);
    }
    _pressedIndex = QModelIndex();
    event->accept();
}

void ElaComboBoxView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (!event->isAutoRepeat() && currentIndex().isValid())
        {
            Q_EMIT itemPressed(currentIndex());
        }
        event->accept();
        return;
    }
    QListView::keyPressEvent(event);
}
