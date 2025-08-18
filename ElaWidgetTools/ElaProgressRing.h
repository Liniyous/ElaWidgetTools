#ifndef ELAFRAMEWORK_ELAWIDGETTOOLS_INCLUDE_ELAPROGRESSRING_H_
#define ELAFRAMEWORK_ELAWIDGETTOOLS_INCLUDE_ELAPROGRESSRING_H_

#include "ElaDef.h"

#include <QWidget>

class ElaProgressRingPrivate;
class ELA_EXPORT ElaProgressRing : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaProgressRing);
    Q_PROPERTY_CREATE_Q_H(bool, IsBusying)
    Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)
    Q_PROPERTY_CREATE_Q_H(bool, IsDisplayValue)
    Q_PROPERTY_CREATE_Q_H(ElaProgressRingType::ValueDisplayMode, ValueDisplayMode)
    Q_PROPERTY_CREATE_Q_H(int, BusyingWidth)
    Q_PROPERTY_CREATE_Q_H(int, BusyingDurationTime)
    Q_PROPERTY_CREATE_Q_H(int, Minimum)
    Q_PROPERTY_CREATE_Q_H(int, Maximum)
    Q_PROPERTY_CREATE_Q_H(int, Value)
    Q_PROPERTY_CREATE_Q_H(int, ValuePixelSize)
public:
    explicit ElaProgressRing(QWidget* parent = nullptr);
    ~ElaProgressRing() override;

    void setRange(int min, int max);

Q_SIGNALS:
    Q_SIGNAL void rangeChanged(int min, int max);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif //ELAFRAMEWORK_ELAWIDGETTOOLS_INCLUDE_ELAPROGRESSRING_H_
