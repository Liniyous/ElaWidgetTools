#ifndef ELALINEEDITPRIVATE_H
#define ELALINEEDITPRIVATE_H

#include <QObject>
#include <QSize>

#include "Def.h"
#include "stdafx.h"
class ElaEvent;
class ElaLineEdit;
class ElaLineEditPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaLineEdit)
    Q_PROPERTY_CREATE_D(ElaIconType, Awesome)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
public:
    explicit ElaLineEditPrivate(QObject* parent = nullptr);
    ~ElaLineEditPrivate();
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);

private:
    ElaApplicationType::ThemeMode _themeMode;
    ElaEvent* _focusEvent{nullptr};
    qreal _textSpacing{0.5};
    void _initStyle();
};

#endif // ELALINEEDITPRIVATE_H
