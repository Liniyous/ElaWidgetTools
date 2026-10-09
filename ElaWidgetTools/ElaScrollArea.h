#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLAREA_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLAREA_H_

#include <QScrollArea>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaScrollAreaPrivate;
class ELA_EXPORT ElaScrollArea : public QScrollArea
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaScrollArea)
public:
    explicit ElaScrollArea(QWidget* parent = nullptr);
    ~ElaScrollArea();

    void setIsGrabGesture(bool isEnable, qreal mousePressEventDelay = 0.5);

    void setIsOverShoot(Qt::Orientation orientation, bool isEnable);
    bool getIsOverShoot(Qt::Orientation orientation) const;

    void setIsAnimation(Qt::Orientation orientation, bool isAnimation);
    bool getIsAnimation(Qt::Orientation orientation) const;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLAREA_H_
