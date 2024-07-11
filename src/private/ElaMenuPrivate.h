#ifndef ELAMENUPRIVATE_H
#define ELAMENUPRIVATE_H

#include <QImage>
#include <QObject>
#include <QPoint>

#include "stdafx.h"
class ElaMenu;
class ElaMenuStyle;
class ElaMenuPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMenu)
    Q_PROPERTY_CREATE(int, AnimationImagePosY)
public:
    explicit ElaMenuPrivate(QObject* parent = nullptr);
    ~ElaMenuPrivate();

private:
    QImage _animationImage{nullptr};
    bool _isCloseAnimation{false};
    QPoint _mousePressPoint;
    ElaMenuStyle* _menuStyle{nullptr};
};

#endif // ELAMENUPRIVATE_H
