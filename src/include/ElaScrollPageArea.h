#ifndef ELASCROLLPAGEAREA_H
#define ELASCROLLPAGEAREA_H

#include <QWidget>
#include <QStyleOption>

#include "stdafx.h"
class ElaScrollPageAreaPrivate;
class ELA_EXPORT ElaScrollPageArea : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaScrollPageArea)

public:
    explicit ElaScrollPageArea(QWidget* parent = nullptr);
    ~ElaScrollPageArea();

    void setBorderRadius(int topLeft, int topRight, int bottomLeft, int bottomRight);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELASCROLLPAGEAREA_H
