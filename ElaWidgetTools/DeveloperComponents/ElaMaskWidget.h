#ifndef ELAMASKWIDGET_H
#define ELAMASKWIDGET_H

#include <QWidget>

#include "ElaProperty.h"
class ElaMaskWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, MaskAlpha)
public:
    explicit ElaMaskWidget(QWidget* parent = nullptr);
    ~ElaMaskWidget();
    void doMaskAnimation(int endValue);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAMASKWIDGET_H
