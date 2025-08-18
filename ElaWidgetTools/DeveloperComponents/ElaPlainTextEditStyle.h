#ifndef ELAPLAINTEXTEDITSTYLE_H
#define ELAPLAINTEXTEDITSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaPlainTextEditStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
public:
    explicit ElaPlainTextEditStyle(QStyle* style = nullptr);
    ~ElaPlainTextEditStyle();
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAPLAINTEXTEDITSTYLE_H
