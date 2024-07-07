#ifndef ELASUGGESTVIEW_H
#define ELASUGGESTVIEW_H

#include <QListView>

class ElaSuggestView : public QListView
{
    Q_OBJECT
public:
    explicit ElaSuggestView(QWidget* parent = nullptr);
    ~ElaSuggestView();
};

#endif // ELASUGGESTVIEW_H
