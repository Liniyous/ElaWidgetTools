#ifndef ELASCROLLPAGEAREA_H
#define ELASCROLLPAGEAREA_H

#include <QWidget>

#include "stdafx.h"
class ElaScrollPageAreaPrivate;
class ELA_EXPORT ElaScrollPageArea : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaScrollPageArea)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaScrollPageArea(QWidget* parent = nullptr);
    ~ElaScrollPageArea();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELASCROLLPAGEAREA_H
