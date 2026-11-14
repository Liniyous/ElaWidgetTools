#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELACOMBOBOXVIEW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELACOMBOBOXVIEW_H_

#include <QListView>
#include <QPersistentModelIndex>

class ElaComboBoxView : public QListView
{
    Q_OBJECT
public:
    explicit ElaComboBoxView(QWidget* parent = nullptr);
    void setModel(QAbstractItemModel* model) override;

Q_SIGNALS:
    Q_SIGNAL void itemPressed(const QModelIndex& index);
    Q_SIGNAL void modelChanged();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    QPersistentModelIndex _pressedIndex;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELACOMBOBOXVIEW_H_
