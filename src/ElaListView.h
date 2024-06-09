#ifndef ELALISTVIEW_H
#define ELALISTVIEW_H

#include <QListView>

class ElaListView : public QListView
{
    Q_OBJECT
public:
    explicit ElaListView(QWidget* parent = nullptr);
    ~ElaListView();
};

#endif // ELALISTVIEW_H
