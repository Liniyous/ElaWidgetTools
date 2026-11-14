#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATABWIDGET_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATABWIDGET_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"

#include <QIcon>
#include <QWidget>

class ElaTabWidgetPrivate;
class ElaTabBar;
class QMimeData;

class ELA_EXPORT ElaTabWidget : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaTabWidget)
    ELA_PROPERTY_Q_H(bool, IsTabTransparent)
    ELA_PROPERTY_Q_H(bool, IsContainerAcceptDrops)
    ELA_PROPERTY_Q_H(QSize, FloatWidgetSize)
public:
    explicit ElaTabWidget(QWidget* parent = nullptr);
    ~ElaTabWidget() override;
    int addTab(QWidget* page, const QString& text);
    int addTab(QWidget* page, const QIcon& icon, const QString& text);
    int insertTab(int index, QWidget* page, const QString& text);
    int insertTab(int index, QWidget* page, const QIcon& icon, const QString& text, bool isPinned = false);
    void removeTab(int index);
    int count() const;
    QWidget* widget(int index) const;
    int indexOf(QWidget* page) const;
    QWidget* currentWidget() const;
    int currentIndex() const;
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget* page);
    QString tabText(int index) const;
    void setTabText(int index, const QString& text);
    QIcon tabIcon(int index) const;
    void setTabIcon(int index, const QIcon& icon);
    QString tabToolTip(int index) const;
    void setTabToolTip(int index, const QString& text);
    void setTabsClosable(bool closable);
    // 固定模式的标签尺寸 自适应模式使用其宽度作为上限
    void setTabSize(QSize size);
    QSize getTabSize() const;
    void setIsTabWidthAdaptive(bool isAdaptive);
    bool getIsTabWidthAdaptive() const;
    void setIsTabPinned(int index, bool isPinned);
    bool getIsTabPinned(int index) const;
    void setIsTabDragEnabled(bool isEnabled);
    void setIsTransferManaged(bool isManaged);
    void setIsDropIndicatorEnabled(bool isEnabled);
    bool getIsDropIndicatorEnabled() const;
    void setIsSplitDropEnabled(bool isEnabled);
    bool getIsSplitDropEnabled() const;
    void setReturnTabWidget(ElaTabWidget* group);
    ElaTabWidget* getReturnTabWidget() const;
    void setDragScope(QObject* scope);
    QObject* getDragScope() const;
    ElaTabBar* getTabBar(bool pinned = false) const;
    void locateCurrentTab();
    void transferTab(ElaTabWidget* source, QWidget* page, int index, bool isPinned, Qt::DropAction action = Qt::MoveAction);
    ElaTabWidget* detachTab(int index, const QPoint& position);
    static ElaTabWidget* getDragSource(const QMimeData* data);
    static QWidget* getDragPage(const QMimeData* data);

Q_SIGNALS:
    Q_SIGNAL void pTabSizeChanged();
    Q_SIGNAL void pIsTabWidthAdaptiveChanged();
    Q_SIGNAL void currentChanged(int index);
    Q_SIGNAL void currentWidgetChanged(QWidget* page);
    Q_SIGNAL void tabCloseRequested(int index);
    Q_SIGNAL void tabMoved(int from, int to);
    Q_SIGNAL void tabPinnedChanged(int index, bool isPinned);
    Q_SIGNAL void tabContextMenuRequested(int index, const QPoint& globalPosition);
    Q_SIGNAL void tabTransferRequested(ElaTabWidget* source, QWidget* page, int index, bool isPinned, Qt::DropAction action, ElaTabWidgetType::DropArea area);
    Q_SIGNAL void floatingGroupCreated(ElaTabWidget* group);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELATABWIDGET_H_
