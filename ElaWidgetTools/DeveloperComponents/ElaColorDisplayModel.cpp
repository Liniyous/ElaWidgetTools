#include "ElaColorDisplayModel.h"

ElaColorDisplayModel::ElaColorDisplayModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

ElaColorDisplayModel::~ElaColorDisplayModel()
{
}

void ElaColorDisplayModel::appendDisplayColor(QList<QColor> colorList)
{
    beginResetModel();
    _displayColorList.append(colorList);
    endResetModel();
}

void ElaColorDisplayModel::appendDisplayColor(QColor color)
{
    beginResetModel();
    _displayColorList.append(color);
    endResetModel();
}

void ElaColorDisplayModel::removeDisplayColor(int index)
{
    if (index < 0 || index >= _displayColorList.count())
    {
        return;
    }
    beginResetModel();
    _displayColorList.removeAt(index);
    endResetModel();
}

void ElaColorDisplayModel::replaceDisplayColor(QColor color, int index)
{
    if (index < 0 || index >= _displayColorList.count())
    {
        return;
    }
    beginResetModel();
    _displayColorList[index] = color;
    endResetModel();
}

QList<QColor> ElaColorDisplayModel::getDisplayColorList() const
{
    return _displayColorList;
}

QColor ElaColorDisplayModel::getDisplayColor(int index) const
{
    if (index < 0 || index >= _displayColorList.count())
    {
        return QColor();
    }
    return _displayColorList[index];
}

int ElaColorDisplayModel::rowCount(const QModelIndex& parent) const
{
    return _displayColorList.count();
}

QVariant ElaColorDisplayModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole)
    {
        return QVariant::fromValue(_displayColorList[index.row()]);
    }
    return QVariant();
}
