#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELANAVIGATIONSTYLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELANAVIGATIONSTYLE_H_
#include "ElaPropertyMacro.h"
#include <QModelIndex>
#include <QProxyStyle>

#include "ElaWidgetToolsDef.h"
class ElaNavigationNode;
class ElaNavigationView;
class QPropertyAnimation;
class ElaNavigationStyle : public QProxyStyle
{
    Q_OBJECT
    ELA_PROPERTY(qreal, Opacity)
    ELA_PROPERTY(qreal, Rotate)
    ELA_PROPERTY(int, ItemHeight)
    ELA_VARIABLE(ElaNavigationView*, NavigationView)
    ELA_PROPERTY(qreal, LastSelectMarkTop)
    ELA_PROPERTY(qreal, LastSelectMarkBottom)
    ELA_PROPERTY(qreal, SelectMarkTop)
    ELA_PROPERTY(qreal, SelectMarkBottom)
    ELA_VARIABLE(QModelIndex, PressIndex)
public:
    explicit ElaNavigationStyle(QStyle* style = nullptr);
    ~ElaNavigationStyle() override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

    void navigationNodeStateChange(const QVariantMap& data);

private:
    ElaThemeType::ThemeMode _themeMode;
    bool _isSelectMarkDisplay{true};
    int _leftPadding{11};
    int _iconAreaWidth{40};
    int _textRightSpacing{3};
    int _indicatorIconAreaWidth{34};
    ElaNavigationNode* _opacityAnimationTargetNode{nullptr};
    ElaNavigationNode* _expandAnimationTargetNode{nullptr};
    ElaNavigationNode* _lastSelectedNode{nullptr};
    QPropertyAnimation* _lastSelectMarkTopAnimation{nullptr};
    QPropertyAnimation* _lastSelectMarkBottomAnimation{nullptr};
    QPropertyAnimation* _selectMarkTopAnimation{nullptr};
    QPropertyAnimation* _selectMarkBottomAnimation{nullptr};
    bool _compareItemY(ElaNavigationNode* node1, ElaNavigationNode* node2);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELANAVIGATIONSTYLE_H_

