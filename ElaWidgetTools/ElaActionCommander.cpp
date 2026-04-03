#include "ElaActionCommander.h"

#include "ElaActionCommanderPrivate.h"

ElaActionCommand::ElaActionCommand(QObject* parent)
    : QObject(parent)
{
}

ElaActionCommand::~ElaActionCommand()
{
}

Q_SINGLETON_CREATE_CPP(ElaActionCommander)
Q_PROPERTY_CREATE_Q_CPP(ElaActionCommander, int, MaxRouteCount)
ElaActionCommander::ElaActionCommander(QObject* parent)
    : QObject{parent}, d_ptr(new ElaActionCommanderPrivate())
{
    Q_D(ElaActionCommander);
    d->q_ptr = this;
    d->_pMaxRouteCount = 500;
}

ElaActionCommander::~ElaActionCommander()
{
}

void ElaActionCommander::recordCommand(const QString& domainName, ElaActionCommand* command, bool isRedo)
{
    Q_D(ElaActionCommander);
    if (!command)
    {
        return;
    }
    command->setParent(this);
    auto& commandData = d->_commandDomainMap[domainName];
    auto& commandList = commandData.commandList;
    if (commandData.currentIndex <= 0 || commandList.count() == 0)
    {
        commandData.undoState = ElaActionCommanderType::UndoValid;
        commandData.redoState = ElaActionCommanderType::RedoInvalid;
        Q_EMIT commanderStateChanged(domainName, ElaActionCommanderType::UndoValid);
        Q_EMIT commanderStateChanged(domainName, ElaActionCommanderType::RedoInvalid);
    }
    else
    {
        // 超过最大命令数 则移除第一条命令
        if (commandList.count() >= d->_pMaxRouteCount)
        {
            commandList.first()->deleteLater();
            commandList.removeFirst();
            commandData.currentIndex -= 1;
        }
    }
    // 当前索引不位于末尾 则清除索引后的数据
    if (commandData.currentIndex != commandList.count() - 1)
    {
        for (int i = commandData.currentIndex + 1; i < commandList.count() - commandData.currentIndex - 1; i++)
        {
            commandList.at(i)->deleteLater();
        }
        commandList.remove(commandData.currentIndex + 1, commandList.count() - commandData.currentIndex - 1);
        if (commandData.currentIndex > 0)
        {
            commandData.redoState = ElaActionCommanderType::RedoInvalid;
            Q_EMIT commanderStateChanged(domainName, ElaActionCommanderType::RedoInvalid);
        }
    }
    commandList.append(command);
    commandData.currentIndex = commandList.count() - 1;
    if (isRedo)
    {
        // 加入时Redo一次
        command->redo();
    }
}

void ElaActionCommander::clearCommand(const QString& domainName)
{
    Q_D(ElaActionCommander);
    if (!d->_commandDomainMap.contains(domainName))
    {
        return;
    }
    auto& commandData = d->_commandDomainMap[domainName];
    auto& commandList = commandData.commandList;
    commandData.currentIndex = -1;
    for (const auto command: commandList)
    {
        command->deleteLater();
    }
    commandList.clear();
    commandData.undoState = ElaActionCommanderType::UndoInvalid;
    commandData.redoState = ElaActionCommanderType::RedoInvalid;
    Q_EMIT commanderStateChanged(domainName, ElaActionCommanderType::UndoInvalid);
    Q_EMIT commanderStateChanged(domainName, ElaActionCommanderType::RedoInvalid);
}

void ElaActionCommander::undoCommand(const QString& domainName)
{
    Q_D(ElaActionCommander);
    if (!d->_commandDomainMap.contains(domainName))
    {
        return;
    }
    auto& commandData = d->_commandDomainMap[domainName];
    if (commandData.undoState == ElaActionCommanderType::UndoInvalid)
    {
        return;
    }
    auto& commandList = commandData.commandList;
    if (commandList.isEmpty())
    {
        return;
    }
    if (commandData.currentIndex == 0)
    {
        commandData.undoState = ElaActionCommanderType::UndoInvalid;
        Q_EMIT commanderStateChanged(domainName, ElaActionCommanderType::UndoInvalid);
    }
    if (commandData.currentIndex == commandList.size() - 1)
    {
        commandData.redoState = ElaActionCommanderType::RedoValid;
        Q_EMIT commanderStateChanged(domainName, ElaActionCommanderType::RedoValid);
    }
    auto command = commandList[commandData.currentIndex];
    command->undo();
    commandData.currentIndex -= 1;
}

void ElaActionCommander::redoCommand(const QString& domainName)
{
    Q_D(ElaActionCommander);
    if (!d->_commandDomainMap.contains(domainName))
    {
        return;
    }
    auto& commandData = d->_commandDomainMap[domainName];
    if (commandData.redoState == ElaActionCommanderType::RedoInvalid)
    {
        return;
    }
    auto& commandList = commandData.commandList;
    if (commandList.isEmpty())
    {
        return;
    }
    if (commandData.currentIndex <= 0)
    {
        commandData.undoState = ElaActionCommanderType::UndoValid;
        Q_EMIT commanderStateChanged(domainName, ElaActionCommanderType::UndoValid);
    }
    if (commandData.currentIndex == commandList.size() - 2)
    {
        commandData.redoState = ElaActionCommanderType::RedoInvalid;
        Q_EMIT commanderStateChanged(domainName, ElaActionCommanderType::RedoInvalid);
    }
    if (commandData.currentIndex < commandList.size() - 1)
    {
        commandData.currentIndex += 1;
    }
    auto command = commandList[commandData.currentIndex];
    command->redo();
}

ElaActionCommanderType::CommanderState ElaActionCommander::getCommanderUndoState(const QString& domainName) const
{
    Q_D(const ElaActionCommander);
    if (!d->_commandDomainMap.contains(domainName))
    {
        return ElaActionCommanderType::UndoInvalid;
    }
    return d->_commandDomainMap[domainName].undoState;
}

ElaActionCommanderType::CommanderState ElaActionCommander::getCommanderRedoState(const QString& domainName) const
{
    Q_D(const ElaActionCommander);
    if (!d->_commandDomainMap.contains(domainName))
    {
        return ElaActionCommanderType::RedoInvalid;
    }
    return d->_commandDomainMap[domainName].redoState;
}
