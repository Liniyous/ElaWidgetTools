#ifndef ELATOOLBUTTONPRIVATE_H
#define ELATOOLBUTTONPRIVATE_H

#include <QObject>

#include "stdafx.h"
class ElaToolButton;
class ElaToolButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaToolButton)
public:
    explicit ElaToolButtonPrivate(QObject* parent = nullptr);
    ~ElaToolButtonPrivate();
};

#endif // ELATOOLBUTTONPRIVATE_H
