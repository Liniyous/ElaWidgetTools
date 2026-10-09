#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELACONTENTDIALOG_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELACONTENTDIALOG_H_
#include <QAbstractNativeEventFilter>
#include <QDialog>

#include "ElaAppBar.h"
#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaContentDialogPrivate;
class ELA_EXPORT ElaContentDialog : public QDialog
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaContentDialog)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit ElaContentDialog(QWidget* parent);
    ~ElaContentDialog() override;
    Q_SLOT virtual void onLeftButtonClicked();
    Q_SLOT virtual void onMiddleButtonClicked();
    Q_SLOT virtual void onRightButtonClicked();
    void setCentralWidget(QWidget* centralWidget);

    void setLeftButtonText(const QString& text);
    void setMiddleButtonText(const QString& text);
    void setRightButtonText(const QString& text);

    void close();
Q_SIGNALS:
    Q_SIGNAL void leftButtonClicked();
    Q_SIGNAL void middleButtonClicked();
    Q_SIGNAL void rightButtonClicked();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELACONTENTDIALOG_H_

