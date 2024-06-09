#include "ElaMenuModel.h"

#include <QAction>
ElaMenuModel::ElaMenuModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

ElaMenuModel::~ElaMenuModel()
{
}

int ElaMenuModel::rowCount(const QModelIndex& parent) const
{
    return this->_actionList.count();
}

int ElaMenuModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant ElaMenuModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole)
    {
        return QVariant::fromValue(_actionList.at(index.row()));
    }
    return QVariant();
}

void ElaMenuModel::appendAction(QAction* action)
{
    beginResetModel();
    _actionList.append(action);
    endResetModel();
}
