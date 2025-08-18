#ifndef ELASPINBOXSTYLE_H
#define ELASPINBOXSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaSpinBoxStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PRIVATE_CREATE(ElaSpinBoxType::ButtonMode, ButtonMode)
public:
    explicit ElaSpinBoxStyle(QStyle* style = nullptr);
    ~ElaSpinBoxStyle() override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELASPINBOXSTYLE_H
