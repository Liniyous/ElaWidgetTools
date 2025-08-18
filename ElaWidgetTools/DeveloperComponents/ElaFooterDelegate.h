#ifndef ELAFOOTERDELEGATE_H
#define ELAFOOTERDELEGATE_H

#include <QModelIndex>
#include <QStyledItemDelegate>

#include "ElaDef.h"
class ElaNavigationNode;
class QPropertyAnimation;
class ElaBaseListView;
class ElaFooterDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_PRIVATE_CREATE(ElaBaseListView*, ElaListView);
    Q_PRIVATE_CREATE(QModelIndex, PressIndex);

public:
    explicit ElaFooterDelegate(QObject* parent = nullptr);
    ~ElaFooterDelegate();
    void navigationNodeStateChange(QVariantMap data);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    int _leftPadding{11};
    int _iconAreaWidth{40};
    int _textRightSpacing{3};
    int _indicatorIconAreaWidth{24};
    qreal _lastSelectMarkTop{10};
    qreal _lastSelectMarkBottom{10};
    qreal _selectMarkTop{10};
    qreal _selectMarkBottom{10};
    bool _isSelectMarkDisplay{true};
    ElaNavigationNode* _lastSelectedNode{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    QPropertyAnimation* _lastSelectMarkTopAnimation{nullptr};
    QPropertyAnimation* _lastSelectMarkBottomAnimation{nullptr};
    QPropertyAnimation* _selectMarkTopAnimation{nullptr};
    QPropertyAnimation* _selectMarkBottomAnimation{nullptr};
    bool _compareItemY(ElaNavigationNode* node1, ElaNavigationNode* node2);
};

#endif // ELAFOOTERDELEGATE_H
