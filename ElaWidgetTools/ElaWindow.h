#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAWINDOW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAWINDOW_H_

#include "ElaPropertyMacro.h"
#include <QMainWindow>

#include "ElaAppBar.h"
#include "ElaWidgetToolsDef.h"
#include "ElaSuggestBox.h"
class ElaWindowPrivate;
class ELA_EXPORT ElaWindow : public QMainWindow
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaWindow)
    ELA_PROPERTY_Q_H(int, ThemeChangeTime)
    ELA_PROPERTY_Q_H(ElaWindowType::StackSwitchMode, StackSwitchMode)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    Q_INVOKABLE explicit ElaWindow(QWidget* parent = nullptr);
    ~ElaWindow() override;

    void setIsStayTop(bool isStayTop);
    Q_REQUIRED_RESULT bool getIsStayTop() const;
    void setIsFixedSize(bool isFixedSize);
    Q_REQUIRED_RESULT bool getIsFixedSize() const;
    void setIsDefaultClosed(bool isDefaultClosed);
    Q_REQUIRED_RESULT bool getIsDefaultClosed() const;
    void setAppBarHeight(int appBarHeight);
    Q_REQUIRED_RESULT int getAppBarHeight() const;
    void setRibbonHeight(int ribbonHeight);
    Q_REQUIRED_RESULT int getRibbonHeight() const;
    void setIsCentralStackedWidgetTransparent(bool isTransparent);
    Q_REQUIRED_RESULT bool getIsCentralStackedWidgetTransparent() const;
    void setIsAllowPageOpenInNewWindow(bool isAllowPageOpenInNewWindow);
    Q_REQUIRED_RESULT bool getIsAllowPageOpenInNewWindow() const;
    void setIsNavigationBarEnable(bool isVisible);
    Q_REQUIRED_RESULT bool getIsNavigationBarEnable() const;
    void setNavigationBarWidth(int navigationBarWidth);
    Q_REQUIRED_RESULT int getNavigationBarWidth() const;
    void setCurrentStackIndex(int currentStackIndex);
    Q_REQUIRED_RESULT int getCurrentStackIndex() const;
    void setNavigationBarDisplayMode(ElaNavigationType::NavigationDisplayMode navigationBarDisplayMode);
    Q_REQUIRED_RESULT ElaNavigationType::NavigationDisplayMode getNavigationBarDisplayMode() const;
    void setWindowPaintMode(ElaWindowType::PaintMode windowPaintMode);
    Q_REQUIRED_RESULT ElaWindowType::PaintMode getWindowPaintMode() const;

    void moveToCenter();

    void setCustomWidget(ElaAppBarType::CustomArea customArea, QWidget* customWidget, QObject* hitTestObject = nullptr, const QString& hitTestFunctionName = "");
    QWidget* getCustomWidget(ElaAppBarType::CustomArea customArea) const;

    void setCentralCustomWidget(QWidget* customWidget);
    QWidget* getCentralCustomWidget() const;

    void setCustomMenu(QMenu* customMenu);
    QMenu* getCustomMenu() const;

    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(const QPixmap& pix);
    void setUserInfoCardTitle(const QString& title);
    void setUserInfoCardSubTitle(const QString& subTitle);
    ElaNavigationType::NodeResult addExpanderNode(const QString& expanderTitle, QString& expanderKey, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeResult addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, int keyPoints, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addFooterNode(const QString& footerTitle, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeResult addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addCategoryNode(const QString& categoryTitle, QString& categoryKey);
    ElaNavigationType::NodeResult addCategoryNode(const QString& categoryTitle, QString& categoryKey, const QString& targetExpanderKey);

    void addCentralWidget(QWidget* centralWidget);
    QWidget* getCentralWidget(int index) const;

    bool getNavigationNodeIsExpanded(const QString& expanderKey) const;
    void expandNavigationNode(const QString& expanderKey);
    void collapseNavigationNode(const QString& expanderKey);
    void removeNavigationNode(const QString& nodeKey) const;
    int getPageOpenInNewWindowCount(const QString& nodeKey) const;
    void backtrackNavigationNode(const QString& nodeKey);

    void setNodeKeyPoints(const QString& nodeKey, int keyPoints);
    int getNodeKeyPoints(const QString& nodeKey) const;

    void setNavigationNodeTitle(const QString& nodeKey, const QString& nodeTitle);
    QString getNavigationNodeTitle(const QString& nodeKey) const;

    void navigation(const QString& pageKey);
    int getCurrentNavigationIndex() const;
    QString getCurrentNavigationPageKey() const;

    const QList<ElaSuggestBox::SuggestData>& getNavigationSuggestDataList() const;

    void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags);
    ElaAppBarType::ButtonFlags getWindowButtonFlags() const;

    void setWindowMoviePath(ElaThemeType::ThemeMode themeMode, const QString& moviePath);
    QString getWindowMoviePath(ElaThemeType::ThemeMode themeMode) const;

    void setWindowPixmap(ElaThemeType::ThemeMode themeMode, const QPixmap& pixmap);
    const QPixmap& getWindowPixmap(ElaThemeType::ThemeMode themeMode) const;

    void setWindowMovieRate(qreal rate);
    qreal getWindowMovieRate() const;

    void tabifyDockWidget(QDockWidget* targetDockWidget, QDockWidget* dockWidget);
    void tabifyDockWidget(Qt::DockWidgetArea area, const QString& targetDockTitle, QDockWidget* dockWidget);

Q_SIGNALS:
    Q_SIGNAL void pIsStayTopChanged();
    Q_SIGNAL void pIsFixedSizeChanged();
    Q_SIGNAL void pIsDefaultClosedChanged();
    Q_SIGNAL void pAppBarHeightChanged();
    Q_SIGNAL void pRibbonHeightChanged();
    Q_SIGNAL void pIsCentralStackedWidgetTransparentChanged();
    Q_SIGNAL void pIsAllowPageOpenInNewWindowChanged();
    Q_SIGNAL void pIsNavigationBarEnableChanged();
    Q_SIGNAL void pNavigationBarWidthChanged();
    Q_SIGNAL void pCurrentStackIndexChanged();
    Q_SIGNAL void pNavigationBarDisplayModeChanged();
    Q_SIGNAL void pWindowPaintModeChanged();
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void closeButtonClicked();
    Q_SIGNAL void navigationNodeClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey);
    Q_SIGNAL void customWidgetChanged();
    Q_SIGNAL void centralCustomWidgetChanged();
    Q_SIGNAL void customMenuChanged();
    Q_SIGNAL void pageOpenInNewWindow(QString nodeKey);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual QMenu* createPopupMenu() override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QWidget* centralWidget() const;
    void setCentralWidget(QWidget* widget);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAWINDOW_H_

