#include "ElaContentDialogPrivate.h"

#include "ElaContentDialog.h"
#include "ElaMaskWidget.h"

ElaContentDialogPrivate::ElaContentDialogPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaContentDialogPrivate::~ElaContentDialogPrivate()
{
}

void ElaContentDialogPrivate::_doCloseAnimation(bool isAccept)
{
    Q_Q(ElaContentDialog);
    _maskWidget->doMaskAnimation(0);
    isAccept ? q->accept() : q->reject();
}

void ElaContentDialogPrivate::_moveToCenter()
{
    Q_Q(ElaContentDialog);
    int width = q->width();
    int height = q->height();
    auto globalPos = _maskWidget->mapToGlobal(QPoint{0, 0});
    q->setGeometry(globalPos.x() + (_maskWidget->width() - width) / 2, globalPos.y() + (_maskWidget->height() - height) / 2, width, height);
}
