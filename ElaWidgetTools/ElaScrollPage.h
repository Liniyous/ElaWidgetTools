#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLPAGE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLPAGE_H_

#include <QWidget>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"
class ElaScrollArea;
class ElaText;
class QHBoxLayout;
class ElaScrollPagePrivate;
class ELA_EXPORT ElaScrollPage : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaScrollPage)

public:
    explicit ElaScrollPage(QWidget* parent = nullptr);
    ~ElaScrollPage() override;

    void setTopCustomWidget(QWidget* customWidget);
    QWidget* getTopCustomWidget() const;

    void setBottomCustomWidget(QWidget* customWidget);
    QWidget* getBottomCustomWidget() const;

    void addCentralWidget(QWidget* centralWidget, bool isWidgetResizeable = true, bool isVerticalGrabGesture = true, qreal mousePressEventDelay = 0.0);

    void navigation(int widgetIndex, bool isLogRoute = true);

    void setPageTitleSpacing(int spacing);
    int getPageTitleSpacing() const;
    void setTitleVisible(bool isVisible);

    void setPageTitlePixelSize(int pixelSize);
    int getPageTitlePixelSize() const;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLPAGE_H_
