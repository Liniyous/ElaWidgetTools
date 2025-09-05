#ifndef ELAPIVOTVIEW_H
#define ELAPIVOTVIEW_H

#include "ElaProperty.h"
#include <QListView>
#include <QModelIndex>
class ElaPivotStyle;
class ElaPivotView : public QListView
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, MarkX)
    Q_PRIVATE_CREATE(int, MarkWidth)
    Q_PROPERTY_CREATE(int, MarkAnimationWidth)
    Q_PRIVATE_CREATE(ElaPivotStyle*, PivotStyle)
    Q_PRIVATE_CREATE(QModelIndex, CurrentIndex)
    Q_PRIVATE_CREATE(QRect, CurrentIndexRect)
    Q_PRIVATE_CREATE(bool, IsAnimationFinished)
public:
    explicit ElaPivotView(QWidget* parent = nullptr);
    ~ElaPivotView() override;
    void doCurrentIndexChangedAnimation(const QModelIndex& index);

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAPIVOTVIEW_H
