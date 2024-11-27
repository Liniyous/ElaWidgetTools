#ifndef ELALOADINGDIALOG_H
#define ELALOADINGDIALOG_H
#include <QAbstractNativeEventFilter>
#include <QDialog>

#include <QTimer>

#include "stdafx.h"
#include "ElaProgressRing.h"

class ElaLoadingDialogPrivate;
class ELA_EXPORT ElaLoadingDialog : public QDialog
{
    Q_OBJECT
    Q_Q_CREATE(ElaLoadingDialog)
public:
    explicit ElaLoadingDialog(QWidget* parent);
    ~ElaLoadingDialog();

    void setContentsShow(const QString &mainTitle, const QString &subTitle, int ms = 5000);
    void loadingDone();

Q_SIGNALS:
    Q_SIGNAL void timeout();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
#endif
};

#endif // ELALOADINGDIALOG_H
