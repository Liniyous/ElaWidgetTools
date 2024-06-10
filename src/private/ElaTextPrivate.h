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
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);

private:
    ElaTextType::TextStyle _textStyle{ElaTextType::NoStyle};
    qreal _textSpacing{0.5};
    int _textSize{28};
};

#endif // ELATEXTPRIVATE_H
