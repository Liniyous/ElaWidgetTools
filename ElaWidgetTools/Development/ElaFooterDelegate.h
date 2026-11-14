#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAFOOTERDELEGATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAFOOTERDELEGATE_H_

#include "ElaPropertyMacro.h"
#include <QModelIndex>
#include <QStyledItemDelegate>

#include "ElaWidgetToolsDef.h"
class ElaNavigationNode;
class ElaNavigationMarkAnimation;
class ElaBaseListView;
class ElaFooterDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    ELA_VARIABLE(QModelIndex, PressIndex);

public:
    explicit ElaFooterDelegate(ElaBaseListView* listView, QObject* parent = nullptr);
    ~ElaFooterDelegate();
    void startSelectionAnimation(ElaNavigationNode* previousNode, ElaNavigationNode* selectedNode);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    int _leftPadding{11};
    int _iconAreaWidth{40};
    int _textRightSpacing{3};
    int _indicatorIconAreaWidth{24};
    ElaThemeType::ThemeMode _themeMode;
    ElaNavigationMarkAnimation* _markAnimation{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAFOOTERDELEGATE_H_
