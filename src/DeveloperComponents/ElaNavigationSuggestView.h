#ifndef ELANAVIGATIONSUGGESTVIEW_H
#define ELANAVIGATIONSUGGESTVIEW_H

#include <QListView>

class ElaNavigationSuggestView : public QListView
{
    Q_OBJECT
public:
    explicit ElaNavigationSuggestView(QWidget* parent = nullptr);
    ~ElaNavigationSuggestView();
};

#endif // ELANAVIGATIONSUGGESTVIEW_H
