#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATABBAR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATABBAR_H_

#include <QDrag>
#include <QTabBar>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"
class ElaTabBarPrivate;
class ELA_EXPORT ElaTabBar : public QTabBar
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaTabBar)
public:
    explicit ElaTabBar(QWidget* parent = nullptr);
    ~ElaTabBar() override;

    void setTabSize(QSize tabSize);
    Q_REQUIRED_RESULT QSize getTabSize() const;

    void setTabText(int index, const QString& text);
    // 将内部滚动平滑定位到当前选中Tab 使其位于最左侧
    void locateCurrentTab();

Q_SIGNALS:
    Q_SIGNAL void pTabSizeChanged();
    Q_SIGNAL void tabBarTextChanged(int tabIndex, const QString& tabText);
    Q_SIGNAL void tabDragCreate(QMimeData* mimeData);
    Q_SIGNAL void tabDragEnter(QMimeData* mimeData);
    Q_SIGNAL void tabDragLeave(QMimeData* mimeData);
    Q_SIGNAL void tabDragDrop(QMimeData* mimeData);

protected:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void tabInserted(int index) override;
    void tabRemoved(int index) override;
    void resizeEvent(QResizeEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELATABBAR_H_
