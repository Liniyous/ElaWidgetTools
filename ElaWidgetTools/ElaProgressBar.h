#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROGRESSBAR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROGRESSBAR_H_

#include <QProgressBar>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaProgressBarPrivate;
class ELA_EXPORT ElaProgressBar : public QProgressBar
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaProgressBar)
public:
    explicit ElaProgressBar(QWidget* parent = nullptr);
    ~ElaProgressBar() override;
    void setMinimum(int minimum);
    void setMaximum(int maximum);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAPROGRESSBAR_H_
