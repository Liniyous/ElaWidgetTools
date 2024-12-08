#ifndef ELASCROLLPAGEAREAPRIVATE_H
#define ELASCROLLPAGEAREAPRIVATE_H

#include <QObject>
#include <QColor>
#include <QMargins>

#include "Def.h"
#include "stdafx.h"
class ElaScrollPageArea;
class ElaScrollPageAreaPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaScrollPageArea)

public:
    explicit ElaScrollPageAreaPrivate(QObject* parent = nullptr);
    ~ElaScrollPageAreaPrivate();

private:
    ElaThemeType::ThemeMode _themeMode;

    QString colorToRgbaString(const QColor &color);

    QString getStyleStr();

    int _topLeftRadius{8};
    int _topRightRadius{8};
    int _bottomLeftRadius{8};
    int _bottomRightRadius{8};
};

#endif // ELASCROLLPAGEAREAPRIVATE_H
