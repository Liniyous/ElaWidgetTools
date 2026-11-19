#include "T_ListViewModel.h"

T_ListViewModel::T_ListViewModel(QObject* parent)
    : QAbstractListModel{parent}
{
    _dataList = QStringList{"项目概览", "界面设计规范", "颜色与主题", "图标资源", "按钮与操作", "文本输入", "下拉与多选", "菜单与弹出层", "导航与标签", "列表与表格", "项目资源树", "卡片与布局", "键盘与焦点", "屏幕采集", "录制与回放", "发布检查清单"};
}

T_ListViewModel::~T_ListViewModel()
{
}

int T_ListViewModel::rowCount(const QModelIndex& parent) const
{
    return this->_dataList.count();
}

QVariant T_ListViewModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return _dataList[index.row()];
    }
    return QVariant();
}
