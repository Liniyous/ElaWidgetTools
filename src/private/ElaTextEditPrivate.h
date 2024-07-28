#ifndef ELATEXTEDITPRIVATE_H
#define ELATEXTEDITPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaTextEdit;
class ElaTextEditPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTextEdit)
public:
    explicit ElaTextEditPrivate(QObject* parent = nullptr);
    ~ElaTextEditPrivate();
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    bool _isWrapAnywhere{false};
    ElaTextType::TextStyle _textStyle{ElaTextType::NoStyle};
    qreal _textSpacing{0.5};
};

#endif // ELATEXTEDITPRIVATE_H
