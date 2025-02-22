#include "ElaToolTipPrivate.h"

#include <QEvent>
#include <QPropertyAnimation>
#include <QTimer>

#include "ElaToolTip.h"
ElaToolTipPrivate::ElaToolTipPrivate(QObject* parent)
    : QObject{parent}
{
    _pOpacity = 1;
}

ElaToolTipPrivate::~ElaToolTipPrivate()
{
}

bool ElaToolTipPrivate::eventFilter(QObject* watched, QEvent* event)
{
    Q_Q(ElaToolTip);
    switch (event->type())
    {
    case QEvent::Enter:
    {
        QTimer::singleShot(_pShowDelayMsec, this, [=]() {
            _doShowAnimation();
        });
        if (_pDisplayMsec > -1)
        {
            QTimer::singleShot(_pDisplayMsec, this, [=]() {
                q->hide();
            });
        }
        break;
    }
    case QEvent::Leave:
    {
        QTimer::singleShot(_pHideDelayMsec, this, [=]() {
            q->hide();
        });
        break;
    }
    case QEvent::HoverMove:
    case QEvent::MouseMove:
    {
        _updatePos();
        break;
    }
    default:
    {
        break;
    }
    }
    return QObject::eventFilter(watched, event);
}

void ElaToolTipPrivate::_doShowAnimation()
{
    Q_Q(ElaToolTip);
    QPoint cursorPoint = QCursor::pos();
    q->move(cursorPoint.x() + 10, cursorPoint.y());
    q->show();
    QPropertyAnimation* showAnimation = new QPropertyAnimation(q, "windowOpacity");
    showAnimation->setEasingCurve(QEasingCurve::InOutSine);
    showAnimation->setDuration(250);
    showAnimation->setStartValue(0);
    showAnimation->setEndValue(1);
    showAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaToolTipPrivate::_updatePos()
{
    Q_Q(ElaToolTip);
    if (q->isVisible())
    {
        QPoint cursorPoint = QCursor::pos();
        q->move(cursorPoint.x() + 10, cursorPoint.y());
    }
}
