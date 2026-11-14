#include "ElaFooterDelegate.h"

#include "ElaApplication.h"
#include "ElaBaseListView.h"
#include "ElaColorAnimation.h"
#include "ElaControlStyle.h"
#include "ElaFooterModel.h"
#include "ElaNavigationMarkAnimation.h"
#include "ElaNavigationNode.h"
#include "ElaTheme.h"

#include <QPainter>
#include <QPainterPath>

ElaFooterDelegate::ElaFooterDelegate(ElaBaseListView* listView, QObject* parent)
    : QStyledItemDelegate{parent}
{
    _markAnimation = new ElaNavigationMarkAnimation(listView->viewport(), true, this);
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaFooterDelegate::~ElaFooterDelegate()
{
}

void ElaFooterDelegate::startSelectionAnimation(ElaNavigationNode* previousNode, ElaNavigationNode* selectedNode)
{
    _markAnimation->start(previousNode, selectedNode);
}

void ElaFooterDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    int fontPixelSize = eApp->getFontPixelSize();
    ElaFooterModel* model = dynamic_cast<ElaFooterModel*>(const_cast<QAbstractItemModel*>(index.model()));
    ElaNavigationNode* node = index.data(Qt::UserRole).value<ElaNavigationNode*>();
    // 背景绘制
    QRect itemRect = option.rect;
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    itemRect.setTop(itemRect.top() + 2);
    itemRect.setBottom(itemRect.bottom() - 2);
    QPainterPath path;
    path.addRoundedRect(itemRect, 8, 8);
    const QColor background = ElaControlStyle::getItemBackgroundColor(_themeMode, option.state, index == _pPressIndex);
    painter->fillPath(path, ElaColorAnimation::getColor(option.widget, background, "ItemBackground", index));
    painter->restore();

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    itemRect = option.rect;

    //顶边线绘制
    if (index.row() == 0)
    {
        painter->setPen(ElaThemeColor(_themeMode, BasicBaseLine));
        painter->drawLine(option.rect.x(), itemRect.y() + 1, option.rect.x() + option.rect.width(), itemRect.y() + 1);
    }
    // 图标绘制
    painter->setPen(index == _pPressIndex ? ElaThemeColor(_themeMode, BasicTextNoFocus) : ElaThemeColor(_themeMode, BasicText));
    if (node->getAwesome() != ElaIconType::None)
    {
        painter->save();
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(fontPixelSize + 4);
        painter->setFont(iconFont);
        painter->drawText(QRect(itemRect.x(), itemRect.y(), _iconAreaWidth, itemRect.height()), Qt::AlignCenter, QChar(node->getAwesome()));
        painter->restore();
    }

    _markAnimation->paint(painter, itemRect, node, node == model->getSelectedNode(), ElaThemeColor(_themeMode, PrimaryNormal));
    if (itemRect.width() <= _iconAreaWidth + _indicatorIconAreaWidth)
    {
        painter->restore();
        return;
    }

    if (int keyPoints = node->getKeyPoints())
    {
        // KeyPoints
        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::white);
        painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 10, 10);
        painter->setBrush(ElaThemeColor(_themeMode, StatusDanger));
        painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 9, 9);
        painter->setPen(QPen(Qt::white, 2));
        QFont font = painter->font();
        font.setBold(true);
        if (keyPoints > 99)
        {
            keyPoints = 99;
        }
        if (keyPoints > 9)
        {
            font.setPixelSize(fontPixelSize - 2);
        }
        else
        {
            font.setPixelSize(fontPixelSize - 1);
        }
        painter->setFont(font);
        painter->drawText(keyPoints > 9 ? 248 : 251, itemRect.y() + itemRect.height() / 2 + 4, QString::number(keyPoints));
        painter->restore();
    }

    // 文字绘制
    painter->setPen(index == _pPressIndex ? ElaThemeColor(_themeMode, BasicTextNoFocus) : ElaThemeColor(_themeMode, BasicText));
    QRect textRect;
    if (node->getAwesome() != ElaIconType::None)
    {
        textRect = QRect(itemRect.x() + _iconAreaWidth, itemRect.y(), itemRect.width() - _textRightSpacing - _indicatorIconAreaWidth - _iconAreaWidth, itemRect.height());
    }
    else
    {
        textRect = QRect(itemRect.x() + _leftPadding, itemRect.y(), itemRect.width() - _textRightSpacing - _indicatorIconAreaWidth - _leftPadding, itemRect.height());
    }
    QString text = painter->fontMetrics().elidedText(node->getNodeTitle(), Qt::ElideRight, textRect.width());
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
    painter->restore();
}

QSize ElaFooterDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(40);
    return size;
}
