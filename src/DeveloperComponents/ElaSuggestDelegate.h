#ifndef ELASUGGESTDELEGATE_H
#define ELASUGGESTDELEGATE_H

#include <QStyledItemDelegate>

class ElaSuggestDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ElaSuggestDelegate(QObject* parent = nullptr);
    ~ElaSuggestDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QLinearGradient* _hovergradient{nullptr};
    QLinearGradient* _selectedgradient{nullptr};
    QLinearGradient* _selectedHovergradient{nullptr};
};

#endif // ELASUGGESTDELEGATE_H
