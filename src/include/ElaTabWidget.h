#ifndef ELATABWIDGET_H
#define ELATABWIDGET_H

#include <QTabWidget>

#include "stdafx.h"
class ELA_EXPORT ElaTabWidget : public QTabWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(bool, IsTabTransparent);

public:
    explicit ElaTabWidget(QWidget* parent = nullptr);
    ~ElaTabWidget();
    void setTabPosition(TabPosition position);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELATABWIDGET_H
