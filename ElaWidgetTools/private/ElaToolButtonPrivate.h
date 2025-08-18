#ifndef ELATOOLBUTTONPRIVATE_H
#define ELATOOLBUTTONPRIVATE_H

#include <QObject>

#include "ElaProperty.h"
class ElaToolButton;
class ElaToolButtonStyle;
class ElaToolButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaToolButton)
public:
    explicit ElaToolButtonPrivate(QObject* parent = nullptr);
    ~ElaToolButtonPrivate();

private:
    ElaToolButtonStyle* _toolButtonStyle{nullptr};
};

#endif // ELATOOLBUTTONPRIVATE_H
