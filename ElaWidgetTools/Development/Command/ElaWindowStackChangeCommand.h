#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_COMMAND_ELAWINDOWSTACKCHANGECOMMAND_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_COMMAND_ELAWINDOWSTACKCHANGECOMMAND_H_

#include "ElaActionCommander.h"
#include "ElaPropertyMacro.h"
#include "ElaWindowPrivate.h"
class ElaWindowStackChangeCommand : public ElaActionCommand
{
    Q_OBJECT
    ELA_VARIABLE(int, UndoStackIndex);
    ELA_VARIABLE(int, RedoStackIndex);
    ELA_VARIABLE(ElaWindowPrivate*, WindowPrivate)
public:
    explicit ElaWindowStackChangeCommand(QObject* parent = nullptr);
    ~ElaWindowStackChangeCommand() override;

    void undo() override;
    void redo() override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_COMMAND_ELAWINDOWSTACKCHANGECOMMAND_H_

