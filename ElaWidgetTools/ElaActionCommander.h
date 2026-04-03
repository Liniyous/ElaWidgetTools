#ifndef ELAACTIONCOMMANDER_H
#define ELAACTIONCOMMANDER_H

#include <QObject>
#include <QVariantMap>

#include "ElaDef.h"
#include "ElaSingleton.h"

class ELA_EXPORT ElaActionCommand : public QObject
{
    Q_OBJECT
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
    Q_Q_CREATE(ElaActionCommander)
    Q_SINGLETON_CREATE_H(ElaActionCommander)
    Q_PROPERTY_CREATE_Q_H(int, MaxRouteCount)
private:
    explicit ElaActionCommander(QObject* parent = nullptr);
    ~ElaActionCommander() override;

Q_SIGNALS:
    Q_SIGNAL void commanderStateChanged(const QString& domainName, ElaActionCommanderType::CommanderState state);

public:
    void recordCommand(const QString& domainName, ElaActionCommand* command, bool isRedo = true);
    void clearCommand(const QString& domainName);
    void undoCommand(const QString& domainName);
    void redoCommand(const QString& domainName);

    ElaActionCommanderType::CommanderState getCommanderUndoState(const QString& domainName) const;
    ElaActionCommanderType::CommanderState getCommanderRedoState(const QString& domainName) const;
};

#endif // ELAACTIONCOMMANDER_H
