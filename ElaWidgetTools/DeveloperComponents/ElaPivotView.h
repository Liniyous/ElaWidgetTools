#ifndef ELAPIVOTVIEW_H
#define ELAPIVOTVIEW_H

#include <QListView>

#include "stdafx.h"
class ElaPivotStyle;
class ElaPivotView : public QListView
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, MarkX)
    Q_PRIVATE_CREATE(int, MarkWidth)
    Q_PROPERTY_CREATE(int, MarkAnimationWidth)
    Q_PRIVATE_CREATE(ElaPivotStyle*, PivotStyle)
public:
    explicit ElaPivotView(QWidget* parent = nullptr);
    ~ElaPivotView();
    void doCurrentIndexChangedAnimation(const QModelIndex& index);

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAPIVOTVIEW_H
