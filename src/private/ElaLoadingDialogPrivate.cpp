#include "ElaLoadingDialogPrivate.h"

#include "ElaLoadingDialog.h"
#include "ElaMaskWidget.h"
ElaLoadingDialogPrivate::ElaLoadingDialogPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaLoadingDialogPrivate::~ElaLoadingDialogPrivate()
{
}

void ElaLoadingDialogPrivate::_doCloseAnimation()
{
    Q_Q(ElaLoadingDialog);
    _maskWidget->doMaskAnimation(0);
    q->close();
}
