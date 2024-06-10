#include "ElaMenuDelegate.h"

#include <QPainter>

#include "ElaApplication.h"
#include "ElaMenu.h"
ElaMenuDelegate::ElaMenuDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaMenuDelegate::~ElaMenuDelegate()
{
}

void ElaMenuDelegate::setMenuItemHeight(int height)
{
    this->_menuItemHeight = height;
}

void ElaMenuDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

    // 选中绘制
    QAction* action = index.data(Qt::UserRole).value<QAction*>();
    bool isActionEnable = action->isEnabled();
    QRect itemRect = option.rect;
    itemRect.setTop(itemRect.top() + 2);
    itemRect.setLeft(itemRect.left() + 4);
    itemRect.setRight(itemRect.right() - 4);
    itemRect.setBottom(itemRect.bottom() - 2);
    if (isActionEnable)
    {
        if (option.showDecorationSelected && (option.state & QStyle::State_Selected))
        {
            painter->setPen(Qt::NoPen);
            if (option.state & QStyle::State_MouseOver)
            {
                // 选中时覆盖
                painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xE4, 0xDF, 0xE6) : QColor(0x46, 0x46, 0x46));
                painter->drawRoundedRect(itemRect, 5, 5);
            }
            else
            {
                // 选中
                painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xE4, 0xDF, 0xE6) : QColor(0x46, 0x46, 0x46));
                painter->drawRoundedRect(itemRect, 5, 5);
            }
        }
        else if (option.showDecorationSelected && (option.state & QStyle::State_MouseOver))
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xE4, 0xDF, 0xE6) : QColor(0x46, 0x46, 0x46));
            painter->drawRoundedRect(itemRect, 5, 5);
        }
    }
    // 图标绘制
    painter->save();
    qreal offset = _menuItemHeight * 0.68;
    painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(offset * 0.8);
    painter->setFont(iconFont);
    QString iconText = action->property("ElaIconType").toString();
    painter->drawText(QRect(_iconLeftPadding, itemRect.y(), offset, itemRect.height()), Qt::AlignCenter, iconText);
    painter->restore();

    QString shortCutStr = action->shortcut().toString();
    // 文字绘制
    painter->setPen(isActionEnable ? _themeMode == ElaApplicationType::Light ? Qt::black : Qt::white : QColor(0xAB, 0xB5, 0xD4));
    painter->drawText(QRect(offset + _iconLeftPadding + _textLeftSpacing, itemRect.y(), shortCutStr.isEmpty() ? itemRect.width() * 0.8 : itemRect.width() * 0.4, itemRect.height()), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, action->text());

    // 快捷键绘制
    if (!shortCutStr.isEmpty())
    {
        painter->drawText(QRect(offset + _iconLeftPadding + _textLeftSpacing + itemRect.width() * 0.4, itemRect.y(), itemRect.width() / 3, itemRect.height()), Qt::AlignRight | Qt::AlignVCenter | Qt::TextSingleLine, shortCutStr);
    }

    // 展开图标绘制
    ElaMenu* menu = action->data().value<ElaMenu*>();
    if (menu)
    {
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(_menuItemHeight * 0.45);
        painter->setFont(iconFont);
        painter->drawText(QRect(itemRect.x() + itemRect.width() - itemRect.width() * 0.125, itemRect.y(), offset, itemRect.height()), Qt::AlignVCenter, QChar((unsigned short)ElaIconType::AngleRight));
    }
    painter->restore();
}

QSize ElaMenuDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(_menuItemHeight);
    return size;
}
