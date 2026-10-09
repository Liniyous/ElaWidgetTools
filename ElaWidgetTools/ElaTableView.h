#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATABLEVIEW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATABLEVIEW_H_

#include <QTableView>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaTableViewPrivate;
class ELA_EXPORT ElaTableView : public QTableView
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaTableView)
public:
    explicit ElaTableView(QWidget* parent = nullptr);
    ~ElaTableView();

    void setHeaderMargin(int headerMargin);
    Q_REQUIRED_RESULT int getHeaderMargin() const;

Q_SIGNALS:
    Q_SIGNAL void pHeaderMarginChanged();
    Q_SIGNAL void tableViewShow();
    Q_SIGNAL void tableViewHide();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELATABLEVIEW_H_
