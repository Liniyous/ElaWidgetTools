#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_COMMAND_ELASCROLLPAGEROUTECOMMAND_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_COMMAND_ELASCROLLPAGEROUTECOMMAND_H_

#include "ElaActionCommander.h"
#include "ElaPropertyMacro.h"

class ElaScrollPagePrivate;
class ElaScrollPageRouteCommand : public ElaActionCommand
{
    Q_OBJECT
    ELA_VARIABLE(bool, IsBreadcrumbClicked)
    ELA_VARIABLE(ElaScrollPagePrivate*, ScrollPagePrivate)
    ELA_VARIABLE(int, UndoPageIndex);
    ELA_VARIABLE(int, RedoPageIndex);

    ELA_VARIABLE(QStringList, UndoBreadcrumbList);
    ELA_VARIABLE(QStringList, RedoBreadcrumbList);

public:
    explicit ElaScrollPageRouteCommand(QObject* parent = nullptr);
    ~ElaScrollPageRouteCommand() override;

    void undo() override;
    void redo() override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_COMMAND_ELASCROLLPAGEROUTECOMMAND_H_

