#include "ElaPivotPrivate.h"

#include "ElaPivot.h"
#include "ElaPivotStyle.h"
#include "ElaPivotView.h"
#include <QDebug>
#include <QModelIndex>
#include <QPropertyAnimation>
ElaPivotPrivate::ElaPivotPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaPivotPrivate::~ElaPivotPrivate()
{
}

void ElaPivotPrivate::_checkCurrentIndex()
{
    Q_Q(ElaPivot);
    QModelIndex viewCurrentIndex = _listView->currentIndex();
    if (_listView->doCurrentIndexChangedAnimation(viewCurrentIndex))
    {
        Q_EMIT q->pCurrentIndexChanged();
    }
}
