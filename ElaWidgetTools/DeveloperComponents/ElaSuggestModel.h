#ifndef ELASUGGESTMODEL_H
#define ELASUGGESTMODEL_H

#include <QAbstractListModel>

#include "ElaDef.h"
class ElaSuggestion;
class ElaSuggestModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ElaSuggestModel(QObject* parent = nullptr);
    ~ElaSuggestModel();
    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    void setSearchSuggestion(QVector<ElaSuggestion*> suggestionVector);
    void clearSearchNode();
    ElaSuggestion* getSearchSuggestion(int row);

private:
    QVector<ElaSuggestion*> _suggestionVector; //符合搜索的节点
};

#endif // ELASUGGESTMODEL_H
