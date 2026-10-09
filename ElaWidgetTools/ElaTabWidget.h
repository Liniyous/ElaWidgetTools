#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATABWIDGET_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATABWIDGET_H_

#include <QTabWidget>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"

class ElaTabWidgetPrivate;
class ELA_EXPORT ElaTabWidget : public QTabWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaTabWidget)
    ELA_PROPERTY_Q_H(bool, IsTabTransparent);
    ELA_PROPERTY_Q_H(bool, IsContainerAcceptDrops);
    ELA_PROPERTY_Q_H(QSize, FloatWidgetSize)
public:
    explicit ElaTabWidget(QWidget* parent = nullptr);
    ~ElaTabWidget() override;

    void setTabSize(QSize tabSize);
    Q_REQUIRED_RESULT QSize getTabSize() const;

    void setTabPosition(TabPosition position);
    // 将TabBar内部滚动平滑定位到当前Tab最左侧
    void locateCurrentTab();

Q_SIGNALS:
    Q_SIGNAL void pTabSizeChanged();
    Q_SIGNAL void currentWidgetChanged(QWidget* widget);

protected:
    void paintEvent(QPaintEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void tabInserted(int index) override;

private:
    friend class ElaCustomTabWidget;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELATABWIDGET_H_
