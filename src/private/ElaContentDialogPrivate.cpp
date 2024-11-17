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

void ElaContentDialogPrivate::_doCloseAnimation()
{
    Q_Q(ElaContentDialog);
    _maskWidget->doMaskAnimation(0);
    q->QDialog::close();
}
