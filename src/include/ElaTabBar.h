#ifndef ELATABBAR_H
#define ELATABBAR_H

#include <QDrag>
#include <QTabBar>

#include "stdafx.h"
class ElaTabBarPrivate;
class ELA_EXPORT ElaTabBar : public QTabBar
{
    Q_OBJECT
    Q_Q_CREATE(ElaTabBar)
public:
    explicit ElaTabBar(QWidget* parent = nullptr);
    ~ElaTabBar();
Q_SIGNALS:
    Q_SIGNAL void tabBarPress(int index);
    Q_SIGNAL void tabDragCreate(QDrag* drag);
    Q_SIGNAL void tabDragDrop(const QMimeData* mimeData);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;
};

#endif // ELATABBAR_H
