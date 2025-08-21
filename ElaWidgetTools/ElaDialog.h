#ifndef ELAFRAMEWORK_ELADIALOG_H
#define ELAFRAMEWORK_ELADIALOG_H

#include "ElaAppBar.h"
#include "ElaDef.h"
#include <QDialog>

class ElaDialogPrivate;
class ELA_EXPORT ElaDialog : public QDialog
{
    Q_OBJECT
    Q_Q_CREATE(ElaDialog)
    Q_PROPERTY_CREATE_Q_H(bool, IsStayTop)
    Q_PROPERTY_CREATE_Q_H(bool, IsFixedSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsDefaultClosed)
    Q_PROPERTY_CREATE_Q_H(int, AppBarHeight)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit ElaDialog(QWidget* parent = nullptr);
    ~ElaDialog() override;
    void moveToCenter();

    void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags);
    ElaAppBarType::ButtonFlags getWindowButtonFlags() const;

Q_SIGNALS:
    Q_SIGNAL void routeBackButtonClicked();
    Q_SIGNAL void navigationButtonClicked();
    Q_SIGNAL void themeChangeButtonClicked();
    Q_SIGNAL void closeButtonClicked();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif //ELAFRAMEWORK_ELADIALOG_H
