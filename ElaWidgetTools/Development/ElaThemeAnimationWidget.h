#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELATHEMEANIMATIONWIDGET_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELATHEMEANIMATIONWIDGET_H_

#include <QWidget>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaThemeAnimationWidget : public QWidget
{
    Q_OBJECT
    ELA_PROPERTY(qreal, Radius)
    ELA_PROPERTY(qreal, EndRadius)
    ELA_PROPERTY(QPoint, Center)
    ELA_PROPERTY(QImage, OldWindowBackground)
public:
    explicit ElaThemeAnimationWidget(QWidget* parent = nullptr);
    ~ElaThemeAnimationWidget() override;
    void startAnimation(int msec);
Q_SIGNALS:
    Q_SIGNAL void animationFinished();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELATHEMEANIMATIONWIDGET_H_

