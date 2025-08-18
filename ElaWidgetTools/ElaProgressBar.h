#ifndef ELAPROGRESSBAR_H
#define ELAPROGRESSBAR_H

#include <QProgressBar>

#include "ElaProperty.h"

class ElaProgressBarPrivate;
class ELA_EXPORT ElaProgressBar : public QProgressBar
{
    Q_OBJECT
    Q_Q_CREATE(ElaProgressBar)
public:
    explicit ElaProgressBar(QWidget* parent = nullptr);
    ~ElaProgressBar() override;
    void setMinimum(int minimum);
    void setMaximum(int maximum);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
};

#endif // ELAPROGRESSBAR_H
