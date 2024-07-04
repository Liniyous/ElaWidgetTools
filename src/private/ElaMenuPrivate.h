#ifndef ELAMENUPRIVATE_H
#define ELAMENUPRIVATE_H

#include <QObject>
#include <QPoint>

#include "stdafx.h"
class ElaMenu;
class ElaMenuPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMenu)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit ElaMenuPrivate(QObject* parent = nullptr);
    ~ElaMenuPrivate();

private:
    bool _isCloseAnimation{false};
    QPoint _mousePressPoint;
};

#endif // ELAMENUPRIVATE_H
