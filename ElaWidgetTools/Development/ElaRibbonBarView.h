#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELARIBBONBARVIEW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELARIBBONBARVIEW_H_
#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include <QListView>
#include <QModelIndex>

class ElaRibbonBarModel;
class ElaRibbonBarDelegate;
class ElaRibbonBar;
class ElaRibbonBarView : public QListView
{
    Q_OBJECT
    ELA_VARIABLE(ElaRibbonBarModel*, RibbonBarModel)
    ELA_VARIABLE(ElaRibbonBarDelegate*, RibbonBarDelegate)
public:
    explicit ElaRibbonBarView(QWidget* parent = nullptr);
    ~ElaRibbonBarView() override;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    ElaRibbonBar* _ribbonBar{nullptr};
    ElaThemeType::ThemeMode _themeMode;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELARIBBONBARVIEW_H_

