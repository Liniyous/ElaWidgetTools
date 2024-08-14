#ifndef ELAAPPBAR_H
#define ELAAPPBAR_H

#include <QAbstractNativeEventFilter>
#include <QWidget>

#include "Def.h"
#include "stdafx.h"
class ElaAppBarPrivate;
class ELA_EXPORT ElaAppBar : public QWidget, QAbstractNativeEventFilter
{
    Q_OBJECT
    Q_Q_CREATE(ElaAppBar)
    Q_PROPERTY_CREATE_Q_H(bool, IsStayTop)
    Q_PROPERTY_CREATE_Q_H(bool, IsFixedSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsDefaultClosed)
    Q_PROPERTY_CREATE_Q_H(bool, IsOnlyAllowMinAndClose)
    Q_PROPERTY_CREATE_Q_H(int, AppBarHeight)
    Q_PROPERTY_CREATE_Q_H(QWidget*, CustomWidget)
    Q_PROPERTY_CREATE_Q_H(int, CustomWidgetMaximumWidth)
public:
    explicit ElaAppBar(QWidget* parent = nullptr);
    ~ElaAppBar();

    void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags);
    ElaAppBarType::ButtonFlags getWindowButtonFlags() const;

    void setRouteBackButtonEnable(bool isEnable);

    void closeWindow();
Q_SIGNALS:
    Q_SIGNAL void routeBackButtonClicked();
    Q_SIGNAL void navigationButtonClicked();
    Q_SIGNAL void themeChangeButtonClicked();
    Q_SIGNAL void closeButtonClicked();

protected:
    virtual bool eventFilter(QObject* obj, QEvent* event) override;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    virtual bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;
#else
    virtual bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;
#endif
};

#endif // ELAAPPBAR_H
