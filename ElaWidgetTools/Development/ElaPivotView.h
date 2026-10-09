#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAPIVOTVIEW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAPIVOTVIEW_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"
#include <QListView>
#include <QModelIndex>
class ElaPivotStyle;
class ElaPivotView : public QListView
{
    Q_OBJECT
    ELA_PROPERTY(int, MarkX)
    ELA_VARIABLE(int, MarkWidth)
    ELA_PROPERTY(int, MarkAnimationWidth)
    ELA_VARIABLE(ElaPivotStyle*, PivotStyle)
    ELA_VARIABLE(QRect, CurrentIndexRect)
    ELA_VARIABLE(bool, IsAnimationFinished)
public:
    explicit ElaPivotView(QWidget* parent = nullptr);
    ~ElaPivotView() override;
    bool doCurrentIndexChangedAnimation(const QModelIndex& index);

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void _startMarkAnimation(qreal markXStart, qreal markXEnd, int markWidthStart, int markWidthEnd);
    QModelIndex _markIndex; // mark当前附着的索引 仅用于平移动画
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAPIVOTVIEW_H_
