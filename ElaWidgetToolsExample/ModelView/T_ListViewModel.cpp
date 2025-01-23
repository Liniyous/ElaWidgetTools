#include "T_ListViewModel.h"

T_ListViewModel::T_ListViewModel(QObject* parent)
    : QAbstractListModel{parent}
{
    _dataList.append("最轻一个灵魂 送出玫瑰星光");
    _dataList.append("最轻一句提问 答案是生命的存亡");
    _dataList.append("最轻一颗麦种 由云帆托起");
    _dataList.append("流浪至黑暗的心脏");
    _dataList.append("轻轻的一支航船");
    _dataList.append("归来却被囚困旧港湾");
    _dataList.append("轻轻的一些落叶");
    _dataList.append("飘零在梦萦故乡彼岸");
    _dataList.append("轻轻的虫 森林鸣唱");
    _dataList.append("轻轻的鱼 渡向汪洋");
    _dataList.append("我们是 轻轻轻轻摇曳的篝火");
    _dataList.append("轻轻对天空哼一支歌");
    _dataList.append("轻轻被谁听见了");
    _dataList.append("轻轻的被随意熄灭了");
    _dataList.append("可我要 重重将夜灼伤");
    _dataList.append("可我要 重重将夜点燃");
    _dataList.append("更明亮的 也曾尘埃一样");
    _dataList.append("最轻一段目光 注视年岁漫长");
    _dataList.append("最轻一根琴弦 沉默着致命的声浪");
    _dataList.append("最轻一线悬丝 紧握住锋芒");
    _dataList.append("牵系着存亡的衡量");
    _dataList.append("轻轻的一枚棋子 阻挡在骑士前进路上");
    _dataList.append("轻轻的一双手掌 爱抚的婴孩不再成长");
    _dataList.append("轻轻的虫 童话里唱");
    _dataList.append("轻轻的鱼 向宇宙望");
    _dataList.append("我们是 轻轻轻轻摇曳的篝火");
    _dataList.append("轻轻对天空哼一支歌");
    _dataList.append("轻轻被谁听见了");
    _dataList.append("轻轻的被随意熄灭了");
    _dataList.append("可我要 重重将夜灼伤");
    _dataList.append("可我要 重重将夜点燃");
    _dataList.append("更明亮的 也曾尘埃一样");
    _dataList.append("轻轻时间 重重浩荡纪年");
    _dataList.append("轻轻的文明 重重书写");
    _dataList.append("轻轻身躯 重重对峙黑夜");
    _dataList.append("轻轻的人类 重重思想");
    _dataList.append("向宇宙望 向宇宙望");
    _dataList.append("向远航");
    _dataList.append("最重一粒微光");
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
