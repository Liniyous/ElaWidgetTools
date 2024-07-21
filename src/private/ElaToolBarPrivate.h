#ifndef ELATOOLBARPRIVATE_H
#define ELATOOLBARPRIVATE_H
#include <QObject>

#include "stdafx.h"
class ElaToolBar;
class ElaToolBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaToolBar)
public:
    explicit ElaToolBarPrivate(QObject* parent = nullptr);
    ~ElaToolBarPrivate();
};

#endif // ELATOOLBARPRIVATE_H
