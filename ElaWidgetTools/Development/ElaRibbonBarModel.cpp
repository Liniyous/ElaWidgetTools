#include "ElaRibbonBarModel.h"
#include <QMenu>
#include <QWidget>
ElaRibbonBarModel::ElaRibbonBarModel(QObject* parent)
    : QAbstractListModel(parent)
{
    // 根组
    ElaRibbonGroupData data;
    _ribbonDataList.append(data);
}

ElaRibbonBarModel::~ElaRibbonBarModel()
{
}

QAction* ElaRibbonBarModel::addRibbonAction(const QString& actionName)
{
    auto action = new QAction(actionName, this);
    const int insertIndex = _ribbonDataList[0].ribbonDataList.count();
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);
    _ribbonDataList[0].ribbonDataList.append(action);
    endInsertRows();
    _watchAction(action);
    return action;
}

QAction* ElaRibbonBarModel::addRibbonAction(const QString& actionName, ElaIconType::IconName icon)
{
    auto action = new QAction(actionName, this);
    action->setProperty("ElaRibbonIcon", QChar(icon));
    const int insertIndex = _ribbonDataList[0].ribbonDataList.count();
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);
    _ribbonDataList[0].ribbonDataList.append(action);
    endInsertRows();
    _watchAction(action);
    return action;
}

QAction* ElaRibbonBarModel::addRibbonAction(const QString& actionName, const QIcon& icon)
{
    auto action = new QAction(icon, actionName, this);
    const int insertIndex = _ribbonDataList[0].ribbonDataList.count();
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);
    _ribbonDataList[0].ribbonDataList.append(action);
    endInsertRows();
    _watchAction(action);
    return action;
}

QAction* ElaRibbonBarModel::addRibbonAction(const QString& groupName, const QString& actionName)
{
    auto action = new QAction(actionName, this);
    action->setProperty("ElaRibbonGroupName", groupName);
    ElaRibbonGroupData* targetGroup = nullptr;
    int insertIndex = 0;
    for (auto& groupData: _ribbonDataList)
    {
        if (groupData.groupName == groupName)
        {
            targetGroup = &groupData;
            insertIndex += groupData.ribbonDataList.count();
            break;
        }
        insertIndex += groupData.ribbonDataList.count();
    }
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);
    if (targetGroup)
    {
        targetGroup->ribbonDataList.append(action);
    }
    else
    {
        ElaRibbonGroupData groupData;
        groupData.groupName = groupName;
        groupData.ribbonDataList.append(action);
        _ribbonDataList.append(groupData);
    }
    endInsertRows();
    _watchAction(action);
    return action;
}

QAction* ElaRibbonBarModel::addRibbonAction(const QString& groupName, const QString& actionName, ElaIconType::IconName icon)
{
    auto action = new QAction(actionName, this);
    action->setProperty("ElaRibbonGroupName", groupName);
    action->setProperty("ElaRibbonIcon", QChar(icon));
    ElaRibbonGroupData* targetGroup = nullptr;
    int insertIndex = 0;
    for (auto& groupData: _ribbonDataList)
    {
        if (groupData.groupName == groupName)
        {
            targetGroup = &groupData;
            insertIndex += groupData.ribbonDataList.count();
            break;
        }
        insertIndex += groupData.ribbonDataList.count();
    }
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);
    if (targetGroup)
    {
        targetGroup->ribbonDataList.append(action);
    }
    else
    {
        ElaRibbonGroupData groupData;
        groupData.groupName = groupName;
        groupData.ribbonDataList.append(action);
        _ribbonDataList.append(groupData);
    }
    endInsertRows();
    _watchAction(action);
    return action;
}

QAction* ElaRibbonBarModel::addRibbonAction(const QString& groupName, const QString& actionName, const QIcon& icon)
{
    auto action = new QAction(icon, actionName, this);
    action->setProperty("ElaRibbonGroupName", groupName);
    ElaRibbonGroupData* targetGroup = nullptr;
    int insertIndex = 0;
    for (auto& groupData: _ribbonDataList)
    {
        if (groupData.groupName == groupName)
        {
            targetGroup = &groupData;
            insertIndex += groupData.ribbonDataList.count();
            break;
        }
        insertIndex += groupData.ribbonDataList.count();
    }
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);
    if (targetGroup)
    {
        targetGroup->ribbonDataList.append(action);
    }
    else
    {
        ElaRibbonGroupData groupData;
        groupData.groupName = groupName;
        groupData.ribbonDataList.append(action);
        _ribbonDataList.append(groupData);
    }
    endInsertRows();
    _watchAction(action);
    return action;
}

void ElaRibbonBarModel::removeRibbonAction(QAction* action)
{
    if (!action)
    {
        return;
    }
    auto ribbonGroupName = action->property("ElaRibbonGroupName").toString();
    int actionIndex = 0;
    for (auto& groupData: _ribbonDataList)
    {
        if (groupData.groupName != ribbonGroupName)
        {
            actionIndex += groupData.ribbonDataList.count();
            continue;
        }
        const int groupActionIndex = groupData.ribbonDataList.indexOf(action);
        if (groupActionIndex < 0)
        {
            return;
        }
        const int removeIndex = actionIndex + groupActionIndex;
        beginRemoveRows(QModelIndex(), removeIndex, removeIndex);
        groupData.ribbonDataList.removeAt(groupActionIndex);
        endRemoveRows();
        if (auto indexWidget = action->property("ElaRibbonWidget").value<QWidget*>())
        {
            indexWidget->deleteLater();
        }
        action->deleteLater();
        return;
    }
}

void ElaRibbonBarModel::removeRibbonAction(const QString& groupName, const QString& actionName)
{
    int actionIndex = 0;
    for (auto& groupData: _ribbonDataList)
    {
        if (groupData.groupName != groupName)
        {
            actionIndex += groupData.ribbonDataList.count();
            continue;
        }
        for (int i = 0; i < groupData.ribbonDataList.count(); i++)
        {
            if (groupData.ribbonDataList[i]->text() == actionName)
            {
                const int removeIndex = actionIndex + i;
                QAction* action = groupData.ribbonDataList[i];
                beginRemoveRows(QModelIndex(), removeIndex, removeIndex);
                groupData.ribbonDataList.removeAt(i);
                endRemoveRows();
                action->deleteLater();
                return;
            }
        }
    }
}

const QList<ElaRibbonGroupData>& ElaRibbonBarModel::getRibbonGroupData() const
{
    return _ribbonDataList;
}

QAction* ElaRibbonBarModel::getRibbonAction(int index) const
{
    if (index < 0)
    {
        return nullptr;
    }
    for (const auto& groupData: _ribbonDataList)
    {
        if (groupData.ribbonDataList.count() > index)
        {
            return groupData.ribbonDataList[index];
        }
        index -= groupData.ribbonDataList.count();
    }
    return nullptr;
}

QModelIndex ElaRibbonBarModel::getRibbonActionModelIndex(QAction* ribbonAction) const
{
    if (!ribbonAction)
    {
        return {};
    }
    int actionIndex = 0;
    for (const auto& groupData: _ribbonDataList)
    {
        for (const auto action: groupData.ribbonDataList)
        {
            if (action == ribbonAction)
            {
                return this->index(actionIndex);
            }
            actionIndex++;
        }
    }
    return {};
}

QList<QAction*> ElaRibbonBarModel::getRibbonActions()
{
    QList<QAction*> ribbonActionList;
    for (const auto& ribbonGroup: _ribbonDataList)
    {
        ribbonActionList.append(ribbonGroup.ribbonDataList);
    }
    return ribbonActionList;
}

QList<QAction*> ElaRibbonBarModel::getRibbonActions(const QString& groupName)
{
    QList<QAction*> ribbonActionList;
    for (const auto& ribbonGroup: _ribbonDataList)
    {
        if (ribbonGroup.groupName == groupName)
        {
            ribbonActionList.append(ribbonGroup.ribbonDataList);
        }
    }
    return ribbonActionList;
}

QList<QAction*> ElaRibbonBarModel::getRibbonActions(const QString& groupName, const QString& actionName)
{
    QList<QAction*> ribbonActionList;
    for (const auto& groupData: _ribbonDataList)
    {
        if (groupData.groupName == groupName)
        {
            for (auto action: groupData.ribbonDataList)
            {
                if (action->text() == actionName)
                {
                    ribbonActionList.append(action);
                }
            }
        }
    }
    return ribbonActionList;
}

int ElaRibbonBarModel::rowCount(const QModelIndex& parent) const
{
    int rowCount = 0;
    for (auto& ribbonGroup: _ribbonDataList)
    {
        rowCount += ribbonGroup.ribbonDataList.count();
    }
    return rowCount;
}

QVariant ElaRibbonBarModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
    {
        auto action = getRibbonAction(index.row());
        if (!action)
        {
            return {};
        }
        const auto actionText = action->text();
        // 拉长间距
        return action->menu() ? actionText + "         " : actionText + "      ";
    }
    case Qt::UserRole:
    {
        return QVariant::fromValue(getRibbonAction(index.row()));
    }
    default:
    {
        break;
    }
    }
    return {};
}

void ElaRibbonBarModel::_watchAction(QAction* action)
{
    connect(action, &QAction::changed, this, [this, action]() {
        const auto index = getRibbonActionModelIndex(action);
        if (index.isValid())
        {
            Q_EMIT dataChanged(index, index, {Qt::DisplayRole, Qt::UserRole, Qt::SizeHintRole});
        }
    });
}

Qt::ItemFlags ElaRibbonBarModel::flags(const QModelIndex& index) const
{
    auto result = QAbstractListModel::flags(index);
    const auto action = getRibbonAction(index.row());
    if (!action || !action->isEnabled())
    {
        result &= ~Qt::ItemIsEnabled;
    }
    return result;
}
