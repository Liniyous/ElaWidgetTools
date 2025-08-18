#ifndef ELAPROGRESSBARPRIVATE_H
#define ELAPROGRESSBARPRIVATE_H

#include <QObject>

#include "ElaProperty.h"

class ElaProgressBarStyle;
class QPropertyAnimation;
class ElaProgressBar;
class ElaProgressBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaProgressBar)
public:
    explicit ElaProgressBarPrivate(QObject* parent = nullptr);
    ~ElaProgressBarPrivate();

private:
    bool _isBusyAnimation{false};
    ElaProgressBarStyle* _style{nullptr};
    QPropertyAnimation* _busyAnimation{nullptr};
};

#endif // ELAPROGRESSBARPRIVATE_H
