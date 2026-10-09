#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELADIALOG_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELADIALOG_H_

#include "ElaAppBar.h"
#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include <QDialog>

class ElaDialogPrivate;
class ELA_EXPORT ElaDialog : public QDialog
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaDialog)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit ElaDialog(QWidget* parent = nullptr);
    ~ElaDialog() override;

    void setIsStayTop(bool isStayTop);
    Q_REQUIRED_RESULT bool getIsStayTop() const;
    void setIsFixedSize(bool isFixedSize);
    Q_REQUIRED_RESULT bool getIsFixedSize() const;
    void setIsDefaultClosed(bool isDefaultClosed);
    Q_REQUIRED_RESULT bool getIsDefaultClosed() const;
    void setAppBarHeight(int appBarHeight);
    Q_REQUIRED_RESULT int getAppBarHeight() const;

    void moveToCenter();

    void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags);
    ElaAppBarType::ButtonFlags getWindowButtonFlags() const;

Q_SIGNALS:
    Q_SIGNAL void pIsStayTopChanged();
    Q_SIGNAL void pIsFixedSizeChanged();
    Q_SIGNAL void pIsDefaultClosedChanged();
    Q_SIGNAL void pAppBarHeightChanged();
    Q_SIGNAL void routeBackButtonClicked();
    Q_SIGNAL void navigationButtonClicked();
    Q_SIGNAL void themeChangeButtonClicked();
    Q_SIGNAL void closeButtonClicked();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELADIALOG_H_
