#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELADRAWERAREA_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELADRAWERAREA_H_

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
#include <QWidget>

class ElaDrawerAreaPrivate;
class ELA_EXPORT ElaDrawerArea : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaDrawerArea)
public:
    explicit ElaDrawerArea(QWidget* parent = nullptr);
    ~ElaDrawerArea() override;

    void setBorderRadius(int borderRadius);
    Q_REQUIRED_RESULT int getBorderRadius() const;
    void setHeaderHeight(int headerHeight);
    Q_REQUIRED_RESULT int getHeaderHeight() const;

    void setDrawerHeader(QWidget* widget);

    void addDrawer(QWidget* widget);
    void removeDrawer(QWidget* widget);

    void expand();
    void collapse();

    bool getIsExpand() const;
Q_SIGNALS:
    Q_SIGNAL void pBorderRadiusChanged();
    Q_SIGNAL void pHeaderHeightChanged();
    Q_SIGNAL void expandStateChanged(bool isExpand);
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELADRAWERAREA_H_
