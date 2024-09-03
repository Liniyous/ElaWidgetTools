#ifndef ELACOLORDISPLAYMODEL_H
#define ELACOLORDISPLAYMODEL_H

#include <QAbstractListModel>
#include <QColor>
class ElaColorDisplayModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ElaColorDisplayModel(QObject* parent = nullptr);
    ~ElaColorDisplayModel();
    void appendDisplayColor(QList<QColor> colorList);
    void appendDisplayColor(QColor color);
    void removeDisplayColor(int index);
    void replaceDisplayColor(QColor color, int index);

    QList<QColor> getDisplayColorList() const;
    QColor getDisplayColor(int index) const;

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QList<QColor> _displayColorList;
};

#endif // ELACOLORDISPLAYMODEL_H
