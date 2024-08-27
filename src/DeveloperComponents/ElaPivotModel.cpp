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
        beginResetModel();
        _pivotList.append(pivot);
        endResetModel();
        return;
    }
}

void ElaPivotModel::removePivot(QString pivot)
{
    if (_pivotList.contains(pivot))
    {
        beginResetModel();
        _pivotList.removeAt(_pivotList.lastIndexOf(pivot));
        endResetModel();
    }
}

void ElaPivotModel::removePivot(int index)
{
    if (index >= _pivotList.count())
    {
        return;
    }
    beginResetModel();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    _pivotList.remove(index, _pivotList.count() - index);
#else
    for (int i = _pivotList.count() - 1; i >= 0; i--)
    {
        if (i < index)
        {
            break;
        }
        _pivotList.removeAt(i);
    }
#endif
    endResetModel();
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
