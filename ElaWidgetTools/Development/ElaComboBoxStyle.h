#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOMBOBOXSTYLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOMBOBOXSTYLE_H_

#include "ElaPropertyMacro.h"
#include <QProxyStyle>

#include "ElaWidgetToolsDef.h"
class ElaComboBoxPopup;
class ElaControlAnimation;
class ElaComboBoxStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaComboBoxStyle(QStyle* style = nullptr);
    ~ElaComboBoxStyle() override;
    void setControlAnimation(ElaControlAnimation* animation);
    void setPopup(const ElaComboBoxPopup* popup);
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;

    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaControlAnimation* _controlAnimation{nullptr};
    const ElaComboBoxPopup* _popup{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOMBOBOXSTYLE_H_
