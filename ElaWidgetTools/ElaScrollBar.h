#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLBAR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLBAR_H_

#include <QAbstractScrollArea>
#include <QScrollBar>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"

class ElaScrollBarPrivate;
class ELA_EXPORT ElaScrollBar : public QScrollBar
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaScrollBar)
    ELA_PROPERTY_Q_H(bool, IsAnimation)
    ELA_PROPERTY_Q_H(qreal, SpeedLimit)
public:
    explicit ElaScrollBar(QWidget* parent = nullptr);
    explicit ElaScrollBar(Qt::Orientation orientation, QWidget* parent = nullptr);
    explicit ElaScrollBar(QScrollBar* originScrollBar, QAbstractScrollArea* parent = nullptr);
    ~ElaScrollBar() override;

    // 滚动条厚度 纵向为宽度 横向为高度 单位为逻辑像素 最小为6
    void setScrollBarWidth(int scrollBarWidth);
    int getScrollBarWidth() const;
    void setIsSmoothScroll(bool isSmoothScroll);
    bool getIsSmoothScroll() const;
    void scrollTo(int position);
    void stopSmoothScroll();

Q_SIGNALS:
    Q_SIGNAL void pScrollBarWidthChanged();
    Q_SIGNAL void rangeAnimationFinished();

protected:
    virtual bool event(QEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLBAR_H_
