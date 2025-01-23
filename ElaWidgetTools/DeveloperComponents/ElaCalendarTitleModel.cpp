#include "ElaCalendarTitleModel.h"

ElaCalendarTitleModel::ElaCalendarTitleModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

ElaCalendarTitleModel::~ElaCalendarTitleModel()
{
}

int ElaCalendarTitleModel::rowCount(const QModelIndex& parent) const
{
    return 7;
}

QVariant ElaCalendarTitleModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole)
    {
        switch (index.row())
        {
        case 0:
        {
            return tr("日");
        }
        case 1:
        {
            return tr("一");
        }
        case 2:
        {
            return tr("二");
        }
        case 3:
        {
            return tr("三");
        }
        case 4:
        {
            return tr("四");
        }
        case 5:
        {
            return tr("五");
        }
        case 6:
        {
            return tr("六");
        }
        }
    }
    return QVariant();
}
