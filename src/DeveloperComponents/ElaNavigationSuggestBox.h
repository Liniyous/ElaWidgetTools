#ifndef ELANAVIGATIONSUGGESTBOX_H
#define ELANAVIGATIONSUGGESTBOX_H

#include <QWidget>

#include "stdafx.h"

class ElaNavigationNode;
class ElaNavigationSuggestBoxPrivate;
class ElaNavigationSuggestBox : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaNavigationSuggestBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaNavigationSuggestBox(QWidget* parent = nullptr);
    ~ElaNavigationSuggestBox();
    void setPlaceholderText(QString placeholderText);

Q_SIGNALS:
    Q_SIGNAL void searchNodeClicked(ElaNavigationNode* node, bool isLogClicked = true);

private:
    friend class ElaNavigationBar;
    friend class ElaNavigationBarPrivate;
};

#endif // ELANAVIGATIONSUGGESTBOX_H
