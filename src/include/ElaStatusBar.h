#ifndef ELASTATUSBAR_H
#define ELASTATUSBAR_H

#include <QStatusBar>

#include "stdafx.h"
class ELA_EXPORT ElaStatusBar : public QStatusBar
{
    Q_OBJECT
public:
    explicit ElaStatusBar(QWidget* parent = nullptr);
    ~ElaStatusBar();
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // ELASTATUSBAR_H
