#ifndef ELAWINDOWSTYLE_H
#define ELAWINDOWSTYLE_H

#include <QProxyStyle>

#include "Def.h"
#include "stdafx.h"
class ElaWindowStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaWindowStyle(QStyle* style = nullptr);
    ~ElaWindowStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    ElaApplicationType::ThemeMode _themeMode;
};

#endif // ELAWINDOWSTYLE_H
