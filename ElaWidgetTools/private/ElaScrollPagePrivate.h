#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLPAGEPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLPAGEPRIVATE_H_

#include <QMap>
#include <QObject>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"
class ElaScrollPage;
class ElaScrollArea;
class QHBoxLayout;
class QVBoxLayout;
class ElaText;
class QStackedWidget;
class ElaBreadcrumbBar;
class ElaScrollPagePrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaScrollPage)

public:
    explicit ElaScrollPagePrivate(QObject* parent = nullptr);
    ~ElaScrollPagePrivate() override;

private:
    friend class ElaScrollPageRouteCommand;
    QWidget* _topCustomWidget{nullptr};
    QWidget* _bottomCustomWidget{nullptr};
    QHBoxLayout* _pageTitleLayout{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    QStackedWidget* _centralStackedWidget{nullptr};
    ElaBreadcrumbBar* _breadcrumbBar{nullptr};
    QMap<QString, int> _centralWidgetMap;
    int _navigationTargetIndex{0};
    int _pageTitleSpacing{0};
    bool _isGrabGesture{false};
    void _switchCentralStackIndex(int targetIndex, int lastIndex);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLPAGEPRIVATE_H_
