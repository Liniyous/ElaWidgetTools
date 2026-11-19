#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELASUGGESTMODEL_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELASUGGESTMODEL_H_

#include <QAbstractListModel>

#include "ElaWidgetToolsDef.h"
class ElaSuggestion;
class ElaSuggestModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ElaSuggestModel(QObject* parent = nullptr);
    ~ElaSuggestModel();
    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    void setSearchSuggestion(const QVector<ElaSuggestion*>& suggestionVector);
    ElaSuggestion* getSearchSuggestion(int row);

private:
    QVector<ElaSuggestion*> _suggestionVector; //符合搜索的节点
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELASUGGESTMODEL_H_

