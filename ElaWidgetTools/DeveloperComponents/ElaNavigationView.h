#ifndef ELANAVIGATIONVIEW_H
#define ELANAVIGATIONVIEW_H

#include <QObject>
#include <QTreeView>

class ElaScrollBar;
class ElaNavigationStyle;
class ElaNavigationView : public QTreeView
{
    Q_OBJECT
public:
    explicit ElaNavigationView(QWidget* parent = nullptr);
    ~ElaNavigationView();
    void navigationNodeStateChange(QVariantMap data);

    Q_SLOT void onCustomContextMenuRequested(const QPoint& pos);
Q_SIGNALS:
    Q_SIGNAL void navigationClicked(const QModelIndex& index);
    Q_SIGNAL void navigationOpenNewWindow(QString nodeKey);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
    ElaNavigationStyle* _navigationStyle{nullptr};
};

#endif // ELANAVIGATIONVIEW_H
