#ifndef ELANAVIGATIONSTYLE_H
#define ELANAVIGATIONSTYLE_H
#include <QProxyStyle>

#include "Def.h"
#include "stdafx.h"
class ElaNavigationNode;
class QStyleOptionViewItem;
class ElaNavigationStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, Opacity);

public:
    explicit ElaNavigationStyle(QStyle* style = nullptr);
    ~ElaNavigationStyle();
    void drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
    void drawPanelItemViewItem(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    void navigationNodeStateChange(QVariantMap data);

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaNavigationNode* _animationTargetNode{nullptr};
};

#endif // ELANAVIGATIONSTYLE_H
