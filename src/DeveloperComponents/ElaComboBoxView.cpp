#include "ElaComboBoxView.h"

#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>

#include "ElaApplication.h"
#include "ElaScrollBar.h"
#include "private/ElaComboBoxViewPrivate.h"
ElaComboBoxView::ElaComboBoxView(bool isMultiSelect, QWidget* parent)
    : QListView(parent), d_ptr(new ElaComboBoxViewPrivate())
{
    Q_D(ElaComboBoxView);
    d->q_ptr = this;
    d->_isMultiSelect = isMultiSelect;
    setMaximumHeight(200);
    setStyleSheet(
        "QListView{border:2px solid #DFDFDF;border-radius:8px;background-color: #F9F9F9;}"
        "QListView::item{height: 35px;border:none;}");
    setVerticalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollBar(new ElaScrollBar(this));
    setSelectionMode(QAbstractItemView::NoSelection);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, d, &ElaComboBoxViewPrivate::onThemeChanged);
}

ElaComboBoxView::~ElaComboBoxView()
{
}

void ElaComboBoxView::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaComboBoxView);
    if (d->_isMultiSelect)
    {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid())
        {
            Q_EMIT itemPressed(index);
        }
        event->ignore();
    }
    else
    {
        QListView::mousePressEvent(event);
    }
}
