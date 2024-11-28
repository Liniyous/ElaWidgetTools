#ifndef ELATABWIDGET_H
#define ELATABWIDGET_H

#include <QTabWidget>

#include "stdafx.h"

class ElaTabWidgetPrivate;
class ELA_EXPORT ElaTabWidget : public QTabWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaTabWidget)
    Q_PROPERTY_CREATE(bool, IsTabTransparent);

public:
    explicit ElaTabWidget(QWidget* parent = nullptr);
    ~ElaTabWidget();
    void setTabPosition(TabPosition position);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;
};

#endif // ELATABWIDGET_H
