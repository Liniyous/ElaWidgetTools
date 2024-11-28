#ifndef ELACALENDARTITLEMODEL_H
#define ELACALENDARTITLEMODEL_H

#include <QAbstractListModel>

class ElaCalendarTitleModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ElaCalendarTitleModel(QObject* parent = nullptr);
    ~ElaCalendarTitleModel();

protected:
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
};

#endif // ELACALENDARTITLEMODEL_H
