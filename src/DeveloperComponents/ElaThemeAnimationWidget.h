#ifndef ELATHEMEANIMATIONWIDGET_H
#define ELATHEMEANIMATIONWIDGET_H

#include <QWidget>

#include "stdafx.h"
class ElaThemeAnimationWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, Radius)
    Q_PROPERTY_CREATE(qreal, EndRadius)
    Q_PROPERTY_CREATE(QPoint, Center)
    Q_PROPERTY_CREATE(QImage, OldWindowBackground)
    Q_PROPERTY_CREATE(QImage, NewWindowBackground)
public:
    explicit ElaThemeAnimationWidget(QWidget* parent = nullptr);
    ~ElaThemeAnimationWidget();
    void startAnimation(int msec);
Q_SIGNALS:
    Q_SIGNAL void animationFinished();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELATHEMEANIMATIONWIDGET_H
