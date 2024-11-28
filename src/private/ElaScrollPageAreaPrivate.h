#ifndef ELASCROLLPAGEAREAPRIVATE_H
#define ELASCROLLPAGEAREAPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaScrollPageArea;
class ElaScrollPageAreaPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaScrollPageArea)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit ElaScrollPageAreaPrivate(QObject* parent = nullptr);
    ~ElaScrollPageAreaPrivate();

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELASCROLLPAGEAREAPRIVATE_H
