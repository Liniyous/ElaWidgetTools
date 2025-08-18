#ifndef ELAGRAPHICSVIEW_H
#define ELAGRAPHICSVIEW_H

#include <QGraphicsView>

#include "ElaProperty.h"
class ElaGraphicsViewPrivate;
class ELA_EXPORT ElaGraphicsView : public QGraphicsView
{
    Q_OBJECT
    Q_Q_CREATE(ElaGraphicsView)
    Q_PROPERTY_CREATE_Q_H(qreal, MaxTransform);
    Q_PROPERTY_CREATE_Q_H(qreal, MinTransform);

public:
    explicit ElaGraphicsView(QWidget* parent = nullptr);
    explicit ElaGraphicsView(QGraphicsScene* scene, QWidget* parent = nullptr);
    ~ElaGraphicsView();

protected:
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
};

#endif // ELAGRAPHICSVIEW_H
