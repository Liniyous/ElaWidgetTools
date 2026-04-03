#ifndef ELAACTIONCOMMANDERPRIVATE_H
#define ELAACTIONCOMMANDERPRIVATE_H
#include <QMap>
#include <QObject>
#include <QVector>

#include "ElaDef.h"
class ElaActionCommand;
class ElaActionCommander;
class ElaActionCommanderPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaActionCommander)
    Q_PROPERTY_CREATE_D(int, MaxRouteCount)
public:
    struct ElaActionCommandDomainData {
        int currentIndex{-1};
        ElaActionCommanderType::CommanderState undoState{ElaActionCommanderType::UndoInvalid};
        ElaActionCommanderType::CommanderState redoState{ElaActionCommanderType::RedoInvalid};
        QVector<ElaActionCommand*> commandList;
    };
    explicit ElaActionCommanderPrivate(QObject* parent = nullptr);
    ~ElaActionCommanderPrivate() override;

private:
    QMap<QString, ElaActionCommandDomainData> _commandDomainMap;
};

#endif // ELAACTIONCOMMANDERPRIVATE_H
