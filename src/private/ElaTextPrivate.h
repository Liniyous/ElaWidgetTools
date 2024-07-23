#ifndef ELATEXTPRIVATE_H
#define ELATEXTPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaText;
class ElaTextPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaText)
public:
    explicit ElaTextPrivate(QObject* parent = nullptr);
    ~ElaTextPrivate();
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    bool _isWrapAnywhere{false};
    ElaTextType::TextStyle _textStyle{ElaTextType::NoStyle};
    qreal _textSpacing{0.5};
};

#endif // ELATEXTPRIVATE_H
