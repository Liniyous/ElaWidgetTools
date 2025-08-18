#include "T_IconModel.h"

#include "ElaDef.h"
T_IconModel::T_IconModel(QObject* parent)
    : QAbstractListModel{parent}
{
    _metaEnum = QMetaEnum::fromType<ElaIconType::IconName>();
    _rowCount = _metaEnum.keyCount() - 1;
    _pIsSearchMode = false;
}

T_IconModel::~T_IconModel()
{
}

int T_IconModel::rowCount(const QModelIndex& parent) const
{
    return _rowCount;
}

void T_IconModel::setSearchKeyList(QStringList list)
{
    beginResetModel();
    this->_searchKeyList = list;
    if (_pIsSearchMode)
    {
        _rowCount = this->getSearchKeyList().count();
    }
    else
    {
        _rowCount = _metaEnum.keyCount() - 1;
    }
    endResetModel();
}

QStringList T_IconModel::getSearchKeyList()
{
    return this->_searchKeyList;
}

QVariant T_IconModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole)
    {
        if (!_pIsSearchMode)
        {
            if (index.row() >= _metaEnum.keyCount() - 1)
            {
                return QVariant();
            }
            return QStringList{_metaEnum.key(index.row() + 1), QChar(_metaEnum.value(index.row() + 1))};
        }
        else
        {
            QStringList iconList;
            if (index.row() >= _searchKeyList.count())
            {
                return QVariant();
            }
            iconList.append(_searchKeyList.at(index.row()));
            iconList.append(QChar(_metaEnum.keyToValue(_searchKeyList.at(index.row()).toUtf8().constData())));
            return iconList;
        }
    }
    return QVariant();
}

QString T_IconModel::getIconNameFromModelIndex(const QModelIndex& index) const
{
    QString iconName;
    if (_pIsSearchMode)
    {
        if (index.row() < _searchKeyList.count())
        {
            iconName = QString("ElaIconType::") + _searchKeyList.at(index.row());
        }
    }
    else
    {
        if (index.row() < _metaEnum.keyCount() - 1)
        {
            iconName = QString("ElaIconType::") + _metaEnum.key(index.row() + 1);
        }
    }
    return iconName;
}
