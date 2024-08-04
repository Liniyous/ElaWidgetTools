#ifndef ELABASELISTVIEW_H
#define ELABASELISTVIEW_H

#include <QListView>

class ElaBaseListView : public QListView
{
    Q_OBJECT
public:
    explicit ElaBaseListView(QWidget* parent = nullptr);
    ~ElaBaseListView();
};

#endif // ELABASELISTVIEW_H
