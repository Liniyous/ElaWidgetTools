#ifndef ELADOCKWIDGET_H
#define ELADOCKWIDGET_H

#include <QDockWidget>

#include "ElaProperty.h"
class ElaDockWidgetPrivate;
class ELA_EXPORT ElaDockWidget : public QDockWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaDockWidget)
public:
    explicit ElaDockWidget(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    explicit ElaDockWidget(const QString& title, QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~ElaDockWidget() override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
#ifdef Q_OS_WIN
    virtual bool event(QEvent* event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
#endif
};

#endif // ELADOCKWIDGET_H
