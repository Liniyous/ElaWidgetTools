#ifndef ELAGRAPHICSVIEWPRIVATE_H
#define ELAGRAPHICSVIEWPRIVATE_H

#include <QObject>

#include "ElaProperty.h"
class ElaGraphicsView;
class ElaGraphicsViewPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaGraphicsView)
    Q_PROPERTY_CREATE_D(qreal, MaxTransform);
    Q_PROPERTY_CREATE_D(qreal, MinTransform);

public:
    explicit ElaGraphicsViewPrivate(QObject* parent = nullptr);
    ~ElaGraphicsViewPrivate();

private:
    void _initStyle();
};

#endif // ELAGRAPHICSVIEWPRIVATE_H
