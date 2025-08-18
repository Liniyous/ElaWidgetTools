#ifndef ELALINEEDITSTYLE_H
#define ELALINEEDITSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaLineEditStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaLineEditStyle(QStyle* style = nullptr);
    ~ElaLineEditStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELALINEEDITSTYLE_H
