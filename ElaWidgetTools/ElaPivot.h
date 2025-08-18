#ifndef ELAPIVOT_H
#define ELAPIVOT_H
#include <QWidget>

#include "ElaProperty.h"

class ElaPivotPrivate;
class ELA_EXPORT ElaPivot : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaPivot)
    Q_PROPERTY_CREATE_Q_H(int, TextPixelSize)
    Q_PROPERTY_CREATE_Q_H(int, CurrentIndex)
    Q_PROPERTY_CREATE_Q_H(int, PivotSpacing)
    Q_PROPERTY_CREATE_Q_H(int, MarkWidth)
public:
    explicit ElaPivot(QWidget* parent = nullptr);
    ~ElaPivot();

    void appendPivot(QString pivotTitle);
    void removePivot(QString pivotTitle);

Q_SIGNALS:
    Q_SIGNAL void pivotClicked(int index);
    Q_SIGNAL void pivotDoubleClicked(int index);
};

#endif // ELAPIVOT_H
