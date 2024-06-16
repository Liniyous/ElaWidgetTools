#include "ElaMultiSelectComboBoxPrivate.h"

#include "ElaComboBoxView.h"
#include "ElaMultiSelectComboBox.h"

ElaMultiSelectComboBoxPrivate::ElaMultiSelectComboBoxPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaMultiSelectComboBoxPrivate::~ElaMultiSelectComboBoxPrivate()
{
}

void ElaMultiSelectComboBoxPrivate::onItemPressed(const QModelIndex& index)
{
    if (!_comboView->selectionModel()->isSelected(index))
    {
        _itemSelection[index.row()] = true;
    }
    else
    {
        _itemSelection[index.row()] = false;
    }
    _refreshCurrentIndexs();
}

void ElaMultiSelectComboBoxPrivate::_refreshCurrentIndexs()
{
    Q_Q(ElaMultiSelectComboBox);
    QString str;
    _adjustSelectedVector();
    for (int i = 0; i < q->count(); i++)
    {
        // 该位选中
        QModelIndex index = q->model()->index(i, 0);
        if (_itemSelection[i])
        {
            _comboView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            if (!str.isEmpty())
            {
                str.append(",");
            }
            str.append(q->itemText(i));
        }
        else
        {
            _comboView->selectionModel()->select(index, QItemSelectionModel::Deselect);
        }
    }
    if (str != _currentText)
    {
        q->update();
        _currentText = str;
        _selectedTextList = _currentText.split(",");
        q->setCurrentIndex(-1);
        Q_EMIT q->currentTextListChanged(_selectedTextList);
    }
}

void ElaMultiSelectComboBoxPrivate::_adjustSelectedVector()
{
    Q_Q(ElaMultiSelectComboBox);
    while (_itemSelection.count() < q->count())
    {
        _itemSelection.append(false);
    }
}
