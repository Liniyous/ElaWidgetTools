#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATABBAR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATABBAR_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"

#include <QTabBar>

class ElaTabBarPrivate;
class ElaScrollBar;
class QMimeData;

class ELA_EXPORT ElaTabBar : public QTabBar
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaTabBar)
public:
    explicit ElaTabBar(QWidget* parent = nullptr);
    ~ElaTabBar() override;
    // 固定模式的标签尺寸 自适应模式使用其宽度作为上限
    void setTabSize(QSize tabSize);
    QSize getTabSize() const;
    void setIsTabWidthAdaptive(bool isAdaptive);
    bool getIsTabWidthAdaptive() const;
    void setTabText(int index, const QString& text);
    void setIsActive(bool isActive);
    void setIsPinned(bool isPinned);
    void setIsDragEnabled(bool isEnabled);
    ElaScrollBar* getScrollBar() const;
    void locateCurrentTab();

Q_SIGNALS:
    Q_SIGNAL void pTabSizeChanged();
    Q_SIGNAL void pIsTabWidthAdaptiveChanged();
    Q_SIGNAL void tabBarTextChanged(int index, const QString& text);
    Q_SIGNAL void tabDragRequested(int index);
    Q_SIGNAL void tabDropRequested(const QMimeData* data, int index, Qt::DropAction action);

protected:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    QSize tabSizeHint(int index) const override;
    QSize minimumTabSizeHint(int index) const override;
    void tabLayoutChange() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELATABBAR_H_
