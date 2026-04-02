#include "ElaNavigationRouteCommand.h"

#include "ElaNavigationBar.h"

ElaNavigationRouteCommand::ElaNavigationRouteCommand(QObject* parent)
    : ElaActionCommand(parent)
{
}

ElaNavigationRouteCommand::~ElaNavigationRouteCommand()
{
}

void ElaNavigationRouteCommand::undo()
{
    _pNavigationBar->navigation(_pUndoPageKey, false, false);
}

void ElaNavigationRouteCommand::redo()
{
    _pNavigationBar->navigation(_pRedoPageKey, false, false);
}