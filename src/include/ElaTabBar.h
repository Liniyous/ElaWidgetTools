#ifndef ELATABBAR_H
#define ELATABBAR_H

#include <QTabBar>

#include "stdafx.h"
class ELA_EXPORT ElaTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit ElaTabBar(QWidget* parent = nullptr);
    ~ElaTabBar();
};

#endif // ELATABBAR_H
