#ifndef ELACOMBOBOXVIEW_H
#define ELACOMBOBOXVIEW_H

#include <QListView>

class ElaComboBoxView : public QListView
{
    Q_OBJECT
public:
    explicit ElaComboBoxView(QWidget* parent = nullptr);
    ~ElaComboBoxView();
Q_SIGNALS:
    Q_SIGNAL void itemPressed(const QModelIndex& index);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // ELACOMBOBOXVIEW_H
