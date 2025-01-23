#ifndef ELAWIDGETTOOLS_ELALCDNUMBER_H
#define ELAWIDGETTOOLS_ELALCDNUMBER_H

#include <QLCDNumber>

#include "stdafx.h"

class ElaLCDNumberPrivate;
class ELA_EXPORT ElaLCDNumber : public QLCDNumber
{
    Q_OBJECT
    Q_Q_CREATE(ElaLCDNumber)
    Q_PROPERTY_CREATE_Q_H(bool, IsUseAutoClock)
    Q_PROPERTY_CREATE_Q_H(QString, AutoClockFormat)
    Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)

public:
    explicit ElaLCDNumber(QWidget* parent = nullptr);
    explicit ElaLCDNumber(uint numDigits, QWidget* parent = nullptr);
    ~ElaLCDNumber() override;
};

#endif //ELAWIDGETTOOLS_ELALCDNUMBER_H
