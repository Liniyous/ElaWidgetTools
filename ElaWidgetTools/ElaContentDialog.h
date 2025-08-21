#ifndef ELACONTENTDIALOG_H
#define ELACONTENTDIALOG_H
#include <QAbstractNativeEventFilter>
#include <QDialog>

#include "ElaAppBar.h"
#include "ElaProperty.h"
class ElaContentDialogPrivate;
class ELA_EXPORT ElaContentDialog : public QDialog
{
    Q_OBJECT
    Q_Q_CREATE(ElaContentDialog)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit ElaContentDialog(QWidget* parent);
    ~ElaContentDialog() override;
    Q_SLOT virtual void onLeftButtonClicked();
    Q_SLOT virtual void onMiddleButtonClicked();
    Q_SLOT virtual void onRightButtonClicked();
    void setCentralWidget(QWidget* centralWidget);

    void setLeftButtonText(QString text);
    void setMiddleButtonText(QString text);
    void setRightButtonText(QString text);

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

#endif // ELACONTENTDIALOG_H
