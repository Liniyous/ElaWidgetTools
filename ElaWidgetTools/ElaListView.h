#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELALISTVIEW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELALISTVIEW_H_

#include <QListView>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaListViewPrivate;
class ELA_EXPORT ElaListView : public QListView
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaListView)
public:
    explicit ElaListView(QWidget* parent = nullptr);
    ~ElaListView();

    void setItemHeight(int itemHeight);
    Q_REQUIRED_RESULT int getItemHeight() const;
    void setIsTransparent(bool isTransparent);
    Q_REQUIRED_RESULT bool getIsTransparent() const;

Q_SIGNALS:
    Q_SIGNAL void pItemHeightChanged();
    Q_SIGNAL void pIsTransparentChanged();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELALISTVIEW_H_
