#include "ElaContentDialogPrivate.h"

#include <QApplication>
#include <QScreen>

#include "ElaContentDialog.h"
#include "ElaMaskWidget.h"
ElaContentDialogPrivate::ElaContentDialogPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaContentDialogPrivate::~ElaContentDialogPrivate()
{
}

void ElaContentDialogPrivate::_moveToCenter()
{
    Q_Q(ElaContentDialog);
    if (q->isMaximized() || q->isFullScreen())
    {
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto geometry = q->screen()->availableGeometry();
#else
    auto geometry = qApp->screenAt(q->geometry().center())->geometry();
#endif
    q->setGeometry((geometry.left() + geometry.right() - q->width()) / 2, (geometry.top() + geometry.bottom() - q->height()) / 2, q->width(), q->height());
}

void ElaContentDialogPrivate::_doCloseAnimation()
{
    Q_Q(ElaContentDialog);
    _maskWidget->doMaskAnimation(0);
    q->close();
}
