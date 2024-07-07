#ifndef ELANAVIGATIONSUGGESTBOXPRIVATE_H
#define ELANAVIGATIONSUGGESTBOXPRIVATE_H

#include <QObject>

#include "stdafx.h"
class ElaEvent;
class ElaLineEdit;
class ElaNavigationNode;
class ElaNavigationSuggestModel;
class ElaNavigationSuggestView;
class ElaNavigationSuggestDelegate;
class ElaNavigationSuggestBox;
class ElaNavigationSuggestBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaNavigationSuggestBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit ElaNavigationSuggestBoxPrivate(QObject* parent = nullptr);
    ~ElaNavigationSuggestBoxPrivate();
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
    Q_SLOT void onSearchEditTextEdit(const QString& searchText);
    Q_SLOT void onSearchViewPressed(const QModelIndex& index);

private:
    friend class ElaNavigationBar;
    friend class ElaNavigationBarPrivate;
    ElaEvent* _focusEvent{nullptr};
    ElaLineEdit* _searchEdit{nullptr};
    ElaNavigationSuggestModel* _searchModel{nullptr};
    ElaNavigationSuggestView* _searchView{nullptr};
    ElaNavigationSuggestDelegate* _searchDelegate{nullptr};
    QVector<ElaNavigationNode*> _pageNodeList; //所有页面节点
    void _raiseSearchView();
    void _appendPageNode(ElaNavigationNode* node);
};

#endif // ELANAVIGATIONSUGGESTBOXPRIVATE_H
