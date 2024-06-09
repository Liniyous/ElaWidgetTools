#ifndef ELASPINBOX_H
#define ELASPINBOX_H

#include <QSpinBox>

#include "stdafx.h"
class ElaSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit ElaSpinBox(QWidget* parent = nullptr);
    ~ElaSpinBox();

private:
};

#endif // ELASPINBOX_H
