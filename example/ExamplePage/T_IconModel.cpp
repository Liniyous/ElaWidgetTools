#include "T_IconModel.h"

#include "Def.h"
T_IconModel::T_IconModel(QObject* parent)
    : QAbstractTableModel{parent}
{
    _metaEnum = QMetaEnum::fromType<ElaIconType>();
    _columnCount = 7;
    _rowCount = (_metaEnum.keyCount() - 1) / _columnCount;
    if ((_metaEnum.keyCount() - 1) % _columnCount)
    {
        _rowCount += 1;
    }
    _pIsSearchMode = false;
}

T_IconModel::~T_IconModel()
{
}

int T_IconModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return _rowCount;
}

int T_IconModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return _columnCount;
}

void T_IconModel::setSearchKeyList(QStringList list)
{
    this->_searchKeyList = list;
    if (_pIsSearchMode)
    {
        beginResetModel();
        int rowCount = this->getSearchKeyList().count() / _columnCount;
        if (this->getSearchKeyList().count() % _columnCount != 0)
        {
            rowCount += 1;
        }
        _rowCount = rowCount;
    }
    else
    {
        _rowCount = (_metaEnum.keyCount() - 1) / _columnCount;
        if ((_metaEnum.keyCount() - 1) % _columnCount)
        {
            _rowCount += 1;
        }
    }
    endResetModel();
}

QStringList T_IconModel::getSearchKeyList()
{
    return this->_searchKeyList;
}

void T_IconModel::setColumnCount(int count)
{
    if (count != this->_columnCount)
    {
        beginResetModel();
        this->_columnCount = count;
        if (count == 0)
        {
            this->_rowCount = 0;
        }
        else
        {
            if (this->_pIsSearchMode)
            {
                int rowCount = this->getSearchKeyList().count() / _columnCount;
                if (this->getSearchKeyList().count() % _columnCount != 0)
                {
                    rowCount += 1;
                }
                _rowCount = rowCount;
            }
            else
            {
                _rowCount = (_metaEnum.keyCount() - 1) / _columnCount;
                if ((_metaEnum.keyCount() - 1) % _columnCount)
                {
                    _rowCount += 1;
                }
            }
        }
        endResetModel();
    }
}

QVariant T_IconModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole)
    {
        if (!_pIsSearchMode)
        {
            if (index.row() * _columnCount + index.column() >= _metaEnum.keyCount() - 1)
            {
                return QVariant();
            }
            return QStringList{_metaEnum.key(index.row() * _columnCount + index.column() + 1), QChar(_metaEnum.value(index.row() * _columnCount + index.column() + 1))};
        }
        else
        {
            QStringList searchKeyList = this->_searchKeyList;
            QStringList iconList;
            if (index.row() * _columnCount + index.column() >= searchKeyList.count())
            {
                return QVariant();
            }
            iconList.append(searchKeyList.at(index.row() * _columnCount + index.column()));
            iconList.append(QChar(_metaEnum.keyToValue(searchKeyList.at(index.row() * _columnCount + index.column()).toUtf8().constData())));
            return iconList;
        }
    }
    return QVariant();
}
