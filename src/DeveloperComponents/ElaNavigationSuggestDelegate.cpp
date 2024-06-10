#include "ElaNavigationSuggestDelegate.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaNavigationNode.h"
#include "ElaNavigationSuggestModel.h"
ElaNavigationSuggestDelegate::ElaNavigationSuggestDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _hovergradient = new QLinearGradient(0, 0, 290, 38);
    _hovergradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
    _hovergradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
    _selectedgradient = new QLinearGradient(0, 0, 290, 38);
    _selectedgradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
    _selectedgradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
    _selectedHovergradient = new QLinearGradient(0, 0, 290, 38);
    _selectedHovergradient->setColorAt(0, QColor(0xEC, 0xEC, 0xF3));
    _selectedHovergradient->setColorAt(1, QColor(0xED, 0xEC, 0xF3));
}

ElaNavigationSuggestDelegate::~ElaNavigationSuggestDelegate()
{
}

void ElaNavigationSuggestDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    int margin = 2;
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    ElaNavigationSuggestModel* model = dynamic_cast<ElaNavigationSuggestModel*>(const_cast<QAbstractItemModel*>(index.model()));
    ElaNavigationNode* node = model->getSearchNode(index.row());
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    QRect optionRect = option.rect;
    optionRect.setTopLeft(QPoint(optionRect.left() + margin, optionRect.top() + margin));
    optionRect.setBottomRight(QPoint(optionRect.right() - margin, optionRect.bottom() - margin));
    path.addRoundedRect(optionRect, 8, 8);
    if (option.showDecorationSelected && (option.state & QStyle::State_Selected))
    {
        if (option.state & QStyle::State_MouseOver)
        {
            //选中时覆盖
            painter->fillPath(path, *_selectedHovergradient);
        }
        else
        {
            //选中
            painter->fillPath(path, *_selectedgradient);
        }
    }
    else
    {
        if (option.state & QStyle::State_MouseOver)
        {
            //覆盖时颜色
            painter->fillPath(path, *_hovergradient);
        }
    }
    //图标绘制
    if (node->getAwesome() != ElaIconType::None)
    {
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(17);
        painter->setFont(iconFont);
        painter->drawText(option.rect.x() + 11, option.rect.y() + 26, QChar((unsigned short)node->getAwesome()));
    }
    //文字绘制
    QFont titlefont("Microsoft YaHei", 10);
    titlefont.setHintingPreference(QFont::PreferNoHinting);
    painter->setFont(titlefont);
    painter->drawText(option.rect.x() + 37, option.rect.y() + 25, node->getNodeTitle());
    painter->restore();
}
