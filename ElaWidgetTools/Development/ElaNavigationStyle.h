#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELANAVIGATIONSTYLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELANAVIGATIONSTYLE_H_
#include "ElaPropertyMacro.h"
#include <QModelIndex>
#include <QPointer>
#include <QProxyStyle>

#include "ElaWidgetToolsDef.h"
class ElaNavigationNode;
class ElaNavigationMarkAnimation;
class ElaNavigationView;
class QPropertyAnimation;
class ElaNavigationStyle : public QProxyStyle
{
    Q_OBJECT
    ELA_PROPERTY(qreal, Rotate)
    ELA_PROPERTY(int, ItemHeight)
    ELA_VARIABLE(QModelIndex, PressIndex)
public:
    explicit ElaNavigationStyle(ElaNavigationView* navigationView);
    ~ElaNavigationStyle() override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

    void startExpandAnimation(ElaNavigationNode* node, bool isExpanded);
    void startSelectionAnimation(ElaNavigationNode* previousNode, ElaNavigationNode* selectedNode);

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaNavigationView* _navigationView{nullptr};
    ElaNavigationMarkAnimation* _markAnimation{nullptr};
    int _leftPadding{11};
    int _iconAreaWidth{40};
    int _textRightSpacing{3};
    int _indicatorIconAreaWidth{34};
    QPointer<ElaNavigationNode> _expandAnimationTargetNode;
    QPropertyAnimation* _rotateAnimation{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELANAVIGATIONSTYLE_H_
