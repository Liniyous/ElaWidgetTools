#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPMENT_ELANAVIGATIONMARKANIMATION_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPMENT_ELANAVIGATIONMARKANIMATION_H_

#include <QObject>
#include <QPointer>

class QColor;
class QPainter;
class QRect;
class QVariantAnimation;
class QWidget;
class ElaNavigationNode;

class ElaNavigationMarkAnimation : public QObject
{
    Q_OBJECT
public:
    explicit ElaNavigationMarkAnimation(QWidget* viewport, bool isFooter, QObject* parent);
    void start(ElaNavigationNode* previousNode, ElaNavigationNode* selectedNode);
    void paint(QPainter* painter, const QRect& itemRect, ElaNavigationNode* node, bool isSelected, const QColor& color) const;

private:
    static bool _getIsAbove(ElaNavigationNode* node, ElaNavigationNode* referenceNode);
    QPointer<QWidget> _viewport;
    QPointer<ElaNavigationNode> _previousNode;
    QVariantAnimation* _animation{nullptr};
    bool _isFooter{false};
    bool _isUpward{false};
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPMENT_ELANAVIGATIONMARKANIMATION_H_
