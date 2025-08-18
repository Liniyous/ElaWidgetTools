#ifndef ELANAVIGATIONSTYLE_H
#define ELANAVIGATIONSTYLE_H
#include <QModelIndex>
#include <QProxyStyle>

#include "ElaDef.h"
class ElaNavigationNode;
class ElaNavigationView;
class QPropertyAnimation;
class ElaNavigationStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, Opacity)
    Q_PROPERTY_CREATE(qreal, Rotate)
    Q_PROPERTY_CREATE(int, ItemHeight)
    Q_PRIVATE_CREATE(ElaNavigationView*, NavigationView)
    Q_PROPERTY_CREATE(qreal, LastSelectMarkTop)
    Q_PROPERTY_CREATE(qreal, LastSelectMarkBottom)
    Q_PROPERTY_CREATE(qreal, SelectMarkTop)
    Q_PROPERTY_CREATE(qreal, SelectMarkBottom)
    Q_PRIVATE_CREATE(QModelIndex, PressIndex)
public:
    explicit ElaNavigationStyle(QStyle* style = nullptr);
    ~ElaNavigationStyle() override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

    void navigationNodeStateChange(QVariantMap data);

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

#endif // ELANAVIGATIONSTYLE_H
