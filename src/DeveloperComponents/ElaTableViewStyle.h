#ifndef ELATABLEVIEWSTYLE_H
#define ELATABLEVIEWSTYLE_H
#include <QProxyStyle>

#include "Def.h"
class QStyleOptionViewItem;
class ElaTableViewStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaTableViewStyle(QStyle* style = nullptr);
    ~ElaTableViewStyle();
    void drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
    void drawPanelItemViewRow(const QStyleOption* opt, QPainter* p, const QWidget* w) const;
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    QLinearGradient* _hovergradient{nullptr};
    QLinearGradient* _selectedgradient{nullptr};
    QLinearGradient* _selectedHovergradient{nullptr};
};

#endif // ELATABLEVIEWSTYLE_H
