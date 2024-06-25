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
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);

private:
    QLinearGradient* _hovergradient{nullptr};
    QLinearGradient* _selectedgradient{nullptr};
    QLinearGradient* _selectedHovergradient{nullptr};
    ElaNavigationNode* _animationTargetNode{nullptr};
};

#endif // ELANAVIGATIONSTYLE_H
