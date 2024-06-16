#ifndef ELANAVIGATIONSUGGESTDELEGATE_H
#define ELANAVIGATIONSUGGESTDELEGATE_H

#include <QStyledItemDelegate>

class ElaNavigationSuggestDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ElaNavigationSuggestDelegate(QObject* parent = nullptr);
    ~ElaNavigationSuggestDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QLinearGradient* _hovergradient{nullptr};
    QLinearGradient* _selectedgradient{nullptr};
    QLinearGradient* _selectedHovergradient{nullptr};
};

#endif // ELANAVIGATIONSUGGESTDELEGATE_H
