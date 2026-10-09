#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_COMMAND_ELANAVIGATIONROUTECOMMAND_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_COMMAND_ELANAVIGATIONROUTECOMMAND_H_

#include "ElaActionCommander.h"
#include "ElaPropertyMacro.h"
class ElaNavigationBar;
class ElaNavigationRouteCommand : public ElaActionCommand
{
    Q_OBJECT
    ELA_VARIABLE(ElaNavigationBar*, NavigationBar);
    ELA_VARIABLE(QString, UndoPageKey);
    ELA_VARIABLE(QString, RedoPageKey);

public:
    explicit ElaNavigationRouteCommand(QObject* parent = nullptr);
    ~ElaNavigationRouteCommand() override;

    void undo() override;
    void redo() override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_COMMAND_ELANAVIGATIONROUTECOMMAND_H_

