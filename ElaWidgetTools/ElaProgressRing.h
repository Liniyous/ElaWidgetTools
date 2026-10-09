#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROGRESSRING_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROGRESSRING_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"

#include <QWidget>

class ElaProgressRingPrivate;
class ELA_EXPORT ElaProgressRing : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaProgressRing);
    ELA_PROPERTY_Q_H(bool, IsTransparent)
    ELA_PROPERTY_Q_H(bool, IsDisplayValue)
    ELA_PROPERTY_Q_H(ElaProgressRingType::ValueDisplayMode, ValueDisplayMode)
    ELA_PROPERTY_Q_H(int, ValuePixelSize)
public:
    explicit ElaProgressRing(QWidget* parent = nullptr);
    ~ElaProgressRing() override;

    void setIsBusying(bool isBusying);
    Q_REQUIRED_RESULT bool getIsBusying() const;
    void setBusyingWidth(int width);
    Q_REQUIRED_RESULT int getBusyingWidth() const;
    void setBusyingDurationTime(int busyingDurationTime);
    Q_REQUIRED_RESULT int getBusyingDurationTime() const;
    void setMinimum(int minimum);
    Q_REQUIRED_RESULT int getMinimum() const;
    void setMaximum(int maximum);
    Q_REQUIRED_RESULT int getMaximum() const;
    void setValue(int value);
    Q_REQUIRED_RESULT int getValue() const;

    void setRange(int min, int max);

Q_SIGNALS:
    Q_SIGNAL void pIsBusyingChanged();
    Q_SIGNAL void pBusyingWidthChanged();
    Q_SIGNAL void pBusyingDurationTimeChanged();
    Q_SIGNAL void pMinimumChanged();
    Q_SIGNAL void pMaximumChanged();
    Q_SIGNAL void pValueChanged();
    Q_SIGNAL void rangeChanged(int min, int max);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROGRESSRING_H_
