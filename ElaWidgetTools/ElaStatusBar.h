#ifndef ELASTATUSBAR_H
#define ELASTATUSBAR_H

#include <QStatusBar>

#include "ElaProperty.h"
class ELA_EXPORT ElaStatusBar : public QStatusBar
{
    Q_OBJECT
public:
    explicit ElaStatusBar(QWidget* parent = nullptr);
    ~ElaStatusBar() override;
};

#endif // ELASTATUSBAR_H
