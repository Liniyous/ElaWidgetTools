#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATREEVIEW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATREEVIEW_H_

#include <QTreeView>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaTreeViewPrivate;
class ELA_EXPORT ElaTreeView : public QTreeView
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaTreeView)
public:
    explicit ElaTreeView(QWidget* parent = nullptr);
    ~ElaTreeView();

    void setItemHeight(int itemHeight);
    Q_REQUIRED_RESULT int getItemHeight() const;
    void setHeaderMargin(int headerMargin);
    Q_REQUIRED_RESULT int getHeaderMargin() const;

Q_SIGNALS:
    Q_SIGNAL void pItemHeightChanged();
    Q_SIGNAL void pHeaderMarginChanged();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELATREEVIEW_H_
