#ifndef ELATABWIDGET_H
#define ELATABWIDGET_H

#include <QTabWidget>

#include "ElaProperty.h"

class ElaTabWidgetPrivate;
class ELA_EXPORT ElaTabWidget : public QTabWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaTabWidget)
    Q_PROPERTY_CREATE(bool, IsTabTransparent);

public:
    explicit ElaTabWidget(QWidget* parent = nullptr);
    ~ElaTabWidget() override;
    void setTabPosition(TabPosition position);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;
    virtual void tabInserted(int index);

private:
    friend class ElaCustomTabWidget;
};

#endif // ELATABWIDGET_H
