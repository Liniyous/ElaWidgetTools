#ifndef ELASPINBOXSTYLE_H
#define ELASPINBOXSTYLE_H

#include <QProxyStyle>

#include "Def.h"
class ElaSpinBoxStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaSpinBoxStyle(QStyle* style = nullptr);
    ~ElaSpinBoxStyle();
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELASPINBOXSTYLE_H
