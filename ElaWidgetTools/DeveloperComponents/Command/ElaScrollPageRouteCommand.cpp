#include "ElaScrollPageRouteCommand.h"

#include "ElaBreadcrumbBar.h"
#include "ElaScrollPage.h"
#include "ElaScrollPagePrivate.h"
ElaScrollPageRouteCommand::ElaScrollPageRouteCommand(QObject* parent)
    : ElaActionCommand(parent)
{
    _pIsBreadcrumbClicked = false;
}

ElaScrollPageRouteCommand::~ElaScrollPageRouteCommand()
{
}

void ElaScrollPageRouteCommand::undo()
{
    if (_pIsBreadcrumbClicked)
    {
        int widgetIndex = _pScrollPagePrivate->_centralWidgetMap.value(_pUndoBreadcrumbList.last());
        _pScrollPagePrivate->_switchCentralStackIndex(widgetIndex, _pScrollPagePrivate->_navigationTargetIndex);
        _pScrollPagePrivate->_navigationTargetIndex = widgetIndex;
        _pScrollPagePrivate->_breadcrumbBar->setBreadcrumbList(_pUndoBreadcrumbList);
    }
    else
    {
        _pScrollPagePrivate->q_ptr->navigation(_pUndoPageIndex, false);
    }
}

void ElaScrollPageRouteCommand::redo()
{
    if (_pIsBreadcrumbClicked)
    {
        int widgetIndex = _pScrollPagePrivate->_centralWidgetMap.value(_pRedoBreadcrumbList.last());
        _pScrollPagePrivate->_switchCentralStackIndex(widgetIndex, _pScrollPagePrivate->_navigationTargetIndex);
        _pScrollPagePrivate->_navigationTargetIndex = widgetIndex;
        _pScrollPagePrivate->_breadcrumbBar->setBreadcrumbList(_pRedoBreadcrumbList);
    }
    else
    {
        _pScrollPagePrivate->q_ptr->navigation(_pRedoPageIndex, false);
    }
}