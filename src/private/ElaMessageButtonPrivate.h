#ifndef ELAMESSAGEBUTTONPRIVATE_H
#define ELAMESSAGEBUTTONPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaMessageButton;
class ElaMessageButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMessageButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, BarTitle);
    Q_PROPERTY_CREATE_D(QString, BarText);
    Q_PROPERTY_CREATE_D(int, DisplayMsec);
    Q_PROPERTY_CREATE_D(ElaMessageBarType::MessageMode, MessageMode);
    Q_PROPERTY_CREATE_D(ElaMessageBarType::PositionPolicy, PositionPolicy);

public:
    explicit ElaMessageButtonPrivate(QObject* parent = nullptr);
    ~ElaMessageButtonPrivate();
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);

private:
    int _shadowBorderWidth{3};
    int _penBorderWidth{1};
    ElaApplicationType::ThemeMode _themeMode;
};

#endif // ELAMESSAGEBUTTONPRIVATE_H
