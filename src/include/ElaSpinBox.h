#ifndef ELASPINBOX_H
#define ELASPINBOX_H

#include <QSpinBox>

#include "stdafx.h"
class ELA_EXPORT ElaSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit ElaSpinBox(QWidget* parent = nullptr);
    ~ElaSpinBox();
};

#endif // ELASPINBOX_H
