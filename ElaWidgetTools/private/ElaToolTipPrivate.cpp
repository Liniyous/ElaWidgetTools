#include "ElaToolTipPrivate.h"

#include "ElaTheme.h"
#include "ElaToolTip.h"

#include <QEvent>
#include <QGuiApplication>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>

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
    q->adjustSize();
    q->show();
    _updatePos();
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
        const QMargins margins = eTheme->getShadowMargins();
        QPoint position = cursorPoint + QPoint(16 - margins.left(), 6 - margins.top());
        QScreen* screen = QGuiApplication::screenAt(cursorPoint);
        if (screen)
        {
            const QRect available = screen->availableGeometry();
            position.setX(qBound(available.left(), position.x(), qMax(available.left(), available.right() - q->width() + 1)));
            position.setY(qBound(available.top(), position.y(), qMax(available.top(), available.bottom() - q->height() + 1)));
        }
        q->move(position);
    }
}
