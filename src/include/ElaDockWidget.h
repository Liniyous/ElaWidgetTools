#ifndef ELADOCKWIDGET_H
#define ELADOCKWIDGET_H

#include <QDockWidget>

#include "stdafx.h"
class ElaDockWidgetPrivate;
class ELA_EXPORT ElaDockWidget : public QDockWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaDockWidget)
public:
    explicit ElaDockWidget(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    explicit ElaDockWidget(const QString& title, QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~ElaDockWidget();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELADOCKWIDGET_H
