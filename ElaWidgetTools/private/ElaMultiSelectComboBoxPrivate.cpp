#include "ElaMultiSelectComboBoxPrivate.h"

#include "ElaComboBoxView.h"
#include "ElaMultiSelectComboBox.h"

#include <QAbstractItemModel>
#include <QFontMetrics>
#include <QItemSelectionModel>
#include <QStyle>
#include <algorithm>

ElaMultiSelectComboBoxPrivate::ElaMultiSelectComboBoxPrivate(QObject* parent)
    : QObject(parent)
{
}

void ElaMultiSelectComboBoxPrivate::onItemPressed(const QModelIndex& index)
{
    if (!index.isValid() || !index.flags().testFlag(Qt::ItemIsEnabled) || !index.flags().testFlag(Qt::ItemIsSelectable))
    {
        return;
    }
    _isSelectionInitialized = true;
    _comboView->selectionModel()->select(index, QItemSelectionModel::Toggle);
}

void ElaMultiSelectComboBoxPrivate::onModelChanged()
{
    for (const QMetaObject::Connection& connection: _modelConnections)
    {
        disconnect(connection);
    }
    _modelConnections.clear();
    QAbstractItemModel* model = _comboView->model();
    QItemSelectionModel* selectionModel = _comboView->selectionModel();
    if (model && selectionModel)
    {
        _modelConnections.append(connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &ElaMultiSelectComboBoxPrivate::_refreshSelection));
        _modelConnections.append(connect(model, &QAbstractItemModel::dataChanged, this, &ElaMultiSelectComboBoxPrivate::_refreshSelection));
        _modelConnections.append(connect(model, &QAbstractItemModel::rowsInserted, this, &ElaMultiSelectComboBoxPrivate::_refreshSelection));
        _modelConnections.append(connect(model, &QAbstractItemModel::rowsRemoved, this, &ElaMultiSelectComboBoxPrivate::_refreshSelection));
        _modelConnections.append(connect(model, &QAbstractItemModel::rowsMoved, this, &ElaMultiSelectComboBoxPrivate::_refreshSelection));
        _modelConnections.append(connect(model, &QAbstractItemModel::layoutChanged, this, &ElaMultiSelectComboBoxPrivate::_refreshSelection));
        _modelConnections.append(connect(model, &QAbstractItemModel::modelReset, this, [this]() {
            _isSelectionInitialized = false;
            _refreshSelection();
        }));
    }
    _refreshSelection();
}

void ElaMultiSelectComboBoxPrivate::onCurrentIndexChanged(int index)
{
    if (!_isUpdatingSelection && !_isSelectionInitialized && index >= 0)
    {
        _setSelection(QList<int>{index});
    }
}

void ElaMultiSelectComboBoxPrivate::_setSelection(const QList<int>& rows)
{
    Q_Q(ElaMultiSelectComboBox);
    _isSelectionInitialized = true;
    QItemSelection selection;
    for (int row: rows)
    {
        const QModelIndex index = q->model()->index(row, q->modelColumn(), q->rootModelIndex());
        if (row >= 0 && row < q->count() && index.isValid() && index.flags().testFlag(Qt::ItemIsEnabled) && index.flags().testFlag(Qt::ItemIsSelectable))
        {
            selection.merge(QItemSelection(index, index), QItemSelectionModel::Select);
        }
    }
    _comboView->selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
    _refreshSelection();
}

void ElaMultiSelectComboBoxPrivate::_refreshSelection()
{
    Q_Q(ElaMultiSelectComboBox);
    if (_isUpdatingSelection || !_comboView->selectionModel())
    {
        return;
    }
    _isUpdatingSelection = true;
    const QModelIndexList indexes = _getSelectedIndexes();
    QStringList selectedTextList;
    QVector<bool> selectedRows(q->count(), false);
    for (const QModelIndex& index: indexes)
    {
        selectedRows[index.row()] = true;
        selectedTextList.append(index.data(Qt::DisplayRole).toString());
    }
    const bool isSelectionChanged = selectedRows != _lastSelection;
    const bool isTextChanged = selectedTextList != _selectedTextList;
    _lastSelection = selectedRows;
    _selectedTextList = selectedTextList;
    q->setCurrentIndex(-1);
    _isUpdatingSelection = false;
    q->update();
    if (isSelectionChanged)
    {
        Q_EMIT q->itemSelectionChanged(selectedRows);
    }
    if (isTextChanged)
    {
        Q_EMIT q->currentTextListChanged(selectedTextList);
    }
}

QModelIndexList ElaMultiSelectComboBoxPrivate::_getSelectedIndexes() const
{
    Q_Q(const ElaMultiSelectComboBox);
    QModelIndexList indexes;
    if (!_comboView->selectionModel())
    {
        return indexes;
    }
    for (const QModelIndex& index: _comboView->selectionModel()->selectedIndexes())
    {
        if (index.parent() == q->rootModelIndex() && index.column() == q->modelColumn())
        {
            indexes.append(index);
        }
    }
    std::sort(indexes.begin(), indexes.end(), [](const QModelIndex& left, const QModelIndex& right) {
        return left.row() < right.row();
    });
    return indexes;
}

QVector<ElaMultiSelectComboBoxPrivate::SelectionTag> ElaMultiSelectComboBoxPrivate::_getSelectionTags() const
{
    Q_Q(const ElaMultiSelectComboBox);
    QVector<SelectionTag> tags;
    const QModelIndexList indexes = _getSelectedIndexes();
    const QFontMetrics metrics(q->font());
    const QRect contentRect = q->rect().adjusted(8, 4, -32, -4);
    const int gap = 4;
    const int closeWidth = 20;
    int left = contentRect.left();
    for (int position = 0; position < indexes.size(); ++position)
    {
        const int remaining = indexes.size() - position - 1;
        const int countWidth = remaining > 0 ? metrics.horizontalAdvance(QString("+%1").arg(remaining)) + 16 + gap : 0;
        const QString text = _getSingleLineText(indexes[position].data().toString());
        const int availableWidth = contentRect.right() - left + 1;
        const int tagWidth = qMin(qMin(metrics.horizontalAdvance(text) + closeWidth + 16, 160), availableWidth - countWidth);
        if (tagWidth < closeWidth + 16 + metrics.horizontalAdvance("…"))
        {
            SelectionTag tag;
            tag.hiddenCount = indexes.size() - position;
            tag.text = QString("+%1").arg(tag.hiddenCount);
            tag.rect = QRect(left, contentRect.top(), qMin(availableWidth, metrics.horizontalAdvance(tag.text) + 16), contentRect.height());
            tag.rect = QStyle::visualRect(q->layoutDirection(), q->rect(), tag.rect);
            QStringList hiddenText;
            for (int hidden = position; hidden < indexes.size(); ++hidden)
            {
                hiddenText.append(_getSingleLineText(indexes[hidden].data().toString()));
            }
            tag.toolTip = hiddenText.join("\n");
            if (tag.rect.width() > 0)
            {
                tags.append(tag);
            }
            break;
        }
        SelectionTag tag;
        tag.index = indexes[position];
        tag.rect = QRect(left, contentRect.top(), tagWidth, contentRect.height());
        tag.closeRect = QRect(tag.rect.right() - closeWidth + 1, tag.rect.top(), closeWidth, tag.rect.height());
        tag.rect = QStyle::visualRect(q->layoutDirection(), q->rect(), tag.rect);
        tag.closeRect = QStyle::visualRect(q->layoutDirection(), q->rect(), tag.closeRect);
        tag.text = metrics.elidedText(text, Qt::ElideRight, tagWidth - closeWidth - 12);
        tag.toolTip = text;
        tags.append(tag);
        left += tagWidth + gap;
    }
    return tags;
}

QString ElaMultiSelectComboBoxPrivate::_getSingleLineText(QString text)
{
    text.replace('\r', ' ').replace('\n', ' ');
    return text;
}
