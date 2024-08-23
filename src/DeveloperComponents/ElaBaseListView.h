#ifndef ELABASELISTVIEW_H
#define ELABASELISTVIEW_H

#include <QListView>
#include <QModelIndex>
class ElaBaseListView : public QListView
{
    Q_OBJECT
public:
    explicit ElaBaseListView(QWidget* parent = nullptr);
    ~ElaBaseListView();
Q_SIGNALS:
    Q_SIGNAL void mousePress(const QModelIndex& index);
    Q_SIGNAL void mouseRelease(const QModelIndex& index);
    Q_SIGNAL void mouseDoubleClick(const QModelIndex& index);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
};

#endif // ELABASELISTVIEW_H
