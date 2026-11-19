#include "ElaSuggestModel.h"

ElaSuggestModel::ElaSuggestModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

ElaSuggestModel::~ElaSuggestModel()
{
}

int ElaSuggestModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return _suggestionVector.count();
}

QVariant ElaSuggestModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void ElaSuggestModel::setSearchSuggestion(const QVector<ElaSuggestion*>& suggestionVector)
{
    if (_suggestionVector == suggestionVector)
    {
        return;
    }
    beginResetModel();
    _suggestionVector = suggestionVector;
    endResetModel();
}

ElaSuggestion* ElaSuggestModel::getSearchSuggestion(int row)
{
    if (row >= _suggestionVector.count())
    {
        return nullptr;
    }
    return _suggestionVector[row];
}
