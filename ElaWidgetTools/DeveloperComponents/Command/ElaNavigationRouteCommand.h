#ifndef ELAFRAMEWORK_ELANAVIGATIONROUTECOMMAND_H
#define ELAFRAMEWORK_ELANAVIGATIONROUTECOMMAND_H

#include "ElaActionCommander.h"
class ElaNavigationBar;
class ElaNavigationRouteCommand : public ElaActionCommand
{
    Q_OBJECT
    Q_PRIVATE_CREATE(ElaNavigationBar*, NavigationBar);
    Q_PRIVATE_REF_CREATE(QString, UndoPageKey);
    Q_PRIVATE_REF_CREATE(QString, RedoPageKey);

public:
    explicit ElaNavigationRouteCommand(QObject* parent = nullptr);
    ~ElaNavigationRouteCommand() override;

    void undo() override;
    void redo() override;
};

#endif //ELAFRAMEWORK_ELANAVIGATIONROUTECOMMAND_H
