#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAACTIONCOMMANDERPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAACTIONCOMMANDERPRIVATE_H_
#include "ElaPropertyMacro.h"
#include <QMap>
#include <QObject>
#include <QVector>

#include "ElaWidgetToolsDef.h"
class ElaActionCommand;
class ElaActionCommander;
class ElaText;
class ElaProgressRing;
class ElaActionCommanderModel;
class ElaActionCommanderView;
class ElaActionCommanderPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaActionCommander)
    ELA_PROPERTY_D(int, IsMessageDisplayEnable)
    ELA_PROPERTY_D(int, MessageDisplayMesc)
    ELA_PROPERTY_D(int, MaxCommandCount)
    ELA_PROPERTY_D(bool, IsRunToCommandProcessEventEnable)
public:
    struct ElaActionCommandDomainData {
        bool isBusyMode{false};
        int currentIndex{-1};
        ElaActionCommanderType::CommanderState undoState{ElaActionCommanderType::UndoInvalid};
        ElaActionCommanderType::CommanderState redoState{ElaActionCommanderType::RedoInvalid};
        ElaActionCommanderModel* commanderModel{nullptr};
        QVector<ElaActionCommand*> commandList;
    };
    explicit ElaActionCommanderPrivate(QObject* parent = nullptr);
    ~ElaActionCommanderPrivate() override;

private:
    void _showCommandDesc(const ElaActionCommand* command, bool isRedo, bool isExecute = false) const;
    bool _executeCommand(const QString& domainName, ElaActionCommand* command);
    bool _recordCommand(const QString& domainName, ElaActionCommand* command, bool isRedo);
    void _undoCommand(const QString& domainName, bool isShowMessage);
    void _redoCommand(const QString& domainName, bool isShowMessage);
    void _initDomain(ElaActionCommandDomainData& domainData) const;
    bool _isCommanderValid{true}; // 是否处于可编辑模式
    ElaProgressRing* _commanderBusyRing{nullptr};
    ElaText* _commanderBusyRingValue{nullptr};
    QWidget* _commanderDesign{nullptr};
    ElaActionCommanderView* _commanderView{nullptr};
    QMap<QString, ElaActionCommandDomainData> _commandDomainMap;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAACTIONCOMMANDERPRIVATE_H_
