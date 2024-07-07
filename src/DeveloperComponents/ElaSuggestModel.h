#ifndef ELANAVIGATIONSUGGESTMODEL_H
#define ELANAVIGATIONSUGGESTMODEL_H

#include <QAbstractListModel>

#include "Def.h"
#include "stdafx.h"
class ElaNavigationNode;
class ElaNavigationSuggestModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ElaNavigationSuggestModel(QObject *parent = nullptr);
    ~ElaNavigationSuggestModel();
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void setSearchNode(QVector<ElaNavigationNode *> node);
    void clearSearchNode();
    ElaNavigationNode *getSearchNode(int row);

private:
    QVector<ElaNavigationNode *> _nodesVector;  //符合搜索的节点
};

#endif  // ELANAVIGATIONSUGGESTMODEL_H
