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
    _pNavigationBar->navigation(_pUndoPageKey, false, true);
    setCommandDesc("撤销路由跳转操作");
}

void ElaNavigationRouteCommand::redo()
{
    _pNavigationBar->navigation(_pRedoPageKey, false, false);
    setCommandDesc("执行路由跳转操作");
}