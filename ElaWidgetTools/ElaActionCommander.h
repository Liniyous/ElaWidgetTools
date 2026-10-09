#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAACTIONCOMMANDER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAACTIONCOMMANDER_H_

#include "ElaPropertyMacro.h"
#include <QObject>
#include <QVariantMap>

#include "ElaWidgetToolsDef.h"
#include "ElaSingletonMacro.h"

class ELA_EXPORT ElaActionCommand : public QObject
{
    Q_OBJECT
    // 是否弹出消息
    ELA_VARIABLE(bool, IsShowMessage)
    // 弹出消息类型
    ELA_VARIABLE(ElaMessageBarType::MessageMode, MessageMode)
    // 弹出消息窗口
    ELA_VARIABLE(QWidget*, MessageWidget)
    // 命令描述
    ELA_VARIABLE(QString, CommandDesc)
public:
    explicit ElaActionCommand(QObject* parent = nullptr);
    ~ElaActionCommand() override;

    virtual void undo() = 0;
    virtual void redo() = 0;
};

class ElaActionCommanderPrivate;
class ELA_EXPORT ElaActionCommander : public QObject
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaActionCommander)
    ELA_SINGLETON_H(ElaActionCommander)
    // 是否启用命令消息提示
    ELA_PROPERTY_Q_H(int, IsMessageDisplayEnable)
    // 命令描述显示时间 单位 ms
    ELA_PROPERTY_Q_H(int, MessageDisplayMesc)
    // 最大命令容量
    ELA_PROPERTY_Q_H(int, MaxCommandCount)
    // 运行至节点时是否在每一次运行间启用事件循环 设置为true时运行较慢 但会同步界面 设置为false时速度快 默认为true
    ELA_PROPERTY_Q_H(bool, IsRunToCommandProcessEventEnable)
    // 当前命令域 只影响Design显示
private:
    explicit ElaActionCommander(QObject* parent = nullptr);
    ~ElaActionCommander() override;

Q_SIGNALS:
    Q_SIGNAL void pCurrentDomainChanged();
    Q_SIGNAL void commanderStateChanged(const QString& domainName, ElaActionCommanderType::CommanderState state);

public:
    void setCurrentDomain(const QString& domainName);
    Q_REQUIRED_RESULT const QString& getCurrentDomain() const;

    bool executeCommand(const QString& domainName, ElaActionCommand* command, bool isInvoke = false);
    bool recordCommand(const QString& domainName, ElaActionCommand* command, bool isRedo = true, bool isInvoke = false);
    void clearCommand(const QString& domainName);
    void undoCommand(const QString& domainName);
    void redoCommand(const QString& domainName);

    void runToCommand(const QString& domainName, int commandIndex);

    ElaActionCommanderType::CommanderState getCommanderUndoState(const QString& domainName) const;
    ElaActionCommanderType::CommanderState getCommanderRedoState(const QString& domainName) const;

    QWidget* getCommanderDesign() const;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAACTIONCOMMANDER_H_
