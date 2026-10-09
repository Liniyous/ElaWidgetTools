#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLBAR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLBAR_H_

#include <QAbstractScrollArea>
#include <QScrollBar>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

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

Q_SIGNALS:
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
