#ifndef ELACOMPACTMODEL_H
#define ELACOMPACTMODEL_H
#include <QAbstractListModel>

#include "Def.h"
#include "stdafx.h"
class ElaNavigationNode;
class ElaCompactModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PRIVATE_CREATE(ElaNavigationNode*, SelectedNode)
public:
    explicit ElaCompactModel(QObject* parent = nullptr);
    ~ElaCompactModel();
    ElaNavigationNode* getNavigationNode(QString nodeKey) const;
    QModelIndex getNavigationModelIndex(ElaNavigationNode* node) const;
    void addCompactNode(ElaNavigationNode* node);

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QList<ElaNavigationNode*> _compactNodeList;
};

#endif // ELACOMPACTMODEL_H
