#include "ElaPivotPrivate.h"

#include <QModelIndex>
#include <QPropertyAnimation>

#include "ElaPivot.h"
#include "ElaPivotStyle.h"
#include "ElaPivotView.h"
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
    QModelIndex currentIndex = _listView->currentIndex();
    if (currentIndex.row() != _listStyle->getCurrentIndex())
    {
        _listView->doCurrentIndexChangedAnimation(currentIndex);
        _listStyle->setCurrentIndex(currentIndex.row());
        Q_EMIT q->pCurrentIndexChanged();
    }
}
