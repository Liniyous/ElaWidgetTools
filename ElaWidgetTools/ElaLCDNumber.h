#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELALCDNUMBER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELALCDNUMBER_H_

#include <QLCDNumber>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaLCDNumberPrivate;
class ELA_EXPORT ElaLCDNumber : public QLCDNumber
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaLCDNumber)
public:
    explicit ElaLCDNumber(QWidget* parent = nullptr);
    explicit ElaLCDNumber(uint numDigits, QWidget* parent = nullptr);
    ~ElaLCDNumber() override;

    void setIsUseAutoClock(bool isUseAutoClock);
    Q_REQUIRED_RESULT bool getIsUseAutoClock() const;
    void setAutoClockFormat(const QString& autoClockFormat);
    Q_REQUIRED_RESULT const QString& getAutoClockFormat() const;
    void setIsTransparent(bool isTransparent);
    Q_REQUIRED_RESULT bool getIsTransparent() const;

protected:
    virtual void paintEvent(QPaintEvent* event) override;

Q_SIGNALS:
    Q_SIGNAL void pIsUseAutoClockChanged();
    Q_SIGNAL void pAutoClockFormatChanged();
    Q_SIGNAL void pIsTransparentChanged();
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELALCDNUMBER_H_
