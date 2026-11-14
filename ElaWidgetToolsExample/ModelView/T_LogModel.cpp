#include "T_LogModel.h"

constexpr int T_LogModel::MaximumLogCount;

T_LogModel::T_LogModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

T_LogModel::~T_LogModel()
{
}

int T_LogModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : _logList.count();
}

QVariant T_LogModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() && index.model() == this && index.row() >= 0 && index.row() < _logList.size() && role == Qt::DisplayRole)
    {
        return _logList[index.row()];
    }
    return QVariant();
}

void T_LogModel::setLogList(QStringList list)
{
    beginResetModel();
    _logList = list.mid(qMax(0, static_cast<int>(list.size()) - MaximumLogCount));
    endResetModel();
}

void T_LogModel::appendLogList(QString log)
{
    appendLogs({log});
}

void T_LogModel::appendLogs(const QStringList& logs)
{
    if (logs.isEmpty())
    {
        return;
    }
    const int logCount = static_cast<int>(logs.size());
    const int firstIncoming = qMax(0, logCount - MaximumLogCount);
    const int incomingCount = logCount - firstIncoming;
    const int removeCount = qMax(0, static_cast<int>(_logList.size()) + incomingCount - MaximumLogCount);
    if (removeCount > 0)
    {
        beginRemoveRows(QModelIndex(), 0, removeCount - 1);
        _logList.erase(_logList.begin(), _logList.begin() + removeCount);
        endRemoveRows();
    }
    const int firstRow = _logList.size();
    beginInsertRows(QModelIndex(), firstRow, firstRow + incomingCount - 1);
    for (int i = firstIncoming; i < logs.size(); ++i)
    {
        _logList.append(logs.at(i));
    }
    endInsertRows();
}

QStringList T_LogModel::getLogList() const
{
    return _logList;
}
