#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPROGRESSBARPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPROGRESSBARPRIVATE_H_

#include <QObject>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaProgressBarStyle;
class QPropertyAnimation;
class ElaProgressBar;
class ElaProgressBarPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaProgressBar)
public:
    explicit ElaProgressBarPrivate(QObject* parent = nullptr);
    ~ElaProgressBarPrivate();

private:
    bool _isBusyAnimation{false};
    ElaProgressBarStyle* _style{nullptr};
    QPropertyAnimation* _busyAnimation{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPROGRESSBARPRIVATE_H_

