#include "ElaPivotModel.h"

ElaPivotModel::ElaPivotModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

ElaPivotModel::~ElaPivotModel()
{
}

void ElaPivotModel::appendPivot(QString pivot)
{
    if (!pivot.isEmpty())
    {
        beginInsertRows(QModelIndex(), _pivotList.count(), _pivotList.count());
        _pivotList.append(pivot);
        endInsertRows();
        return;
    }
}

void ElaPivotModel::removePivot(QString pivot)
{
    if (_pivotList.contains(pivot))
    {
        int index = _pivotList.lastIndexOf(pivot);
        beginRemoveRows(QModelIndex(), index, index);
        _pivotList.removeAt(index);
        endRemoveRows();
    }
}

int ElaPivotModel::getPivotListCount() const
{
    return _pivotList.count();
}

int ElaPivotModel::rowCount(const QModelIndex& parent) const
{
    return _pivotList.count();
}

QVariant ElaPivotModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return _pivotList[index.row()];
    }
    return QVariant();
}
