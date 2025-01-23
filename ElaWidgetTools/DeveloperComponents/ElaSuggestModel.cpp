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

void ElaSuggestModel::setSearchSuggestion(QVector<ElaSuggestion*> suggestionVector)
{
    if (suggestionVector.count() == 0)
    {
        return;
    }
    beginResetModel();
    _suggestionVector = suggestionVector;
    endResetModel();
}

void ElaSuggestModel::clearSearchNode()
{
    this->_suggestionVector.clear();
}

ElaSuggestion* ElaSuggestModel::getSearchSuggestion(int row)
{
    if (row >= _suggestionVector.count())
    {
        return nullptr;
    }
    return _suggestionVector[row];
}
