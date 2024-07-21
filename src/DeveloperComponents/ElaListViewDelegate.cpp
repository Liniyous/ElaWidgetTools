#include "ElaListViewDelegate.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaApplication.h"
ElaListViewDelegate::ElaListViewDelegate(QObject* parent)
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
    _themeMode = eApp->getThemeMode();
    connect(eApp, &ElaApplication::themeModeChanged, this, &ElaListViewDelegate::onThemeChanged);
}

ElaListViewDelegate::~ElaListViewDelegate()
{
}

void ElaListViewDelegate::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    _themeMode = themeMode;
    if (themeMode == ElaApplicationType::Light)
    {
        _hovergradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
        _hovergradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
        _selectedgradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
        _selectedgradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
        _selectedHovergradient->setColorAt(0, QColor(0xEC, 0xEC, 0xF3));
        _selectedHovergradient->setColorAt(1, QColor(0xED, 0xEC, 0xF3));
    }
    else
    {
        _hovergradient->setColorAt(0, QColor(0x35, 0x35, 0x35));
        _hovergradient->setColorAt(1, QColor(0x35, 0x35, 0x35));
        _selectedgradient->setColorAt(0, QColor(0x35, 0x35, 0x35));
        _selectedgradient->setColorAt(1, QColor(0x35, 0x35, 0x35));
        _selectedHovergradient->setColorAt(0, QColor(0x2F, 0x2F, 0x2F));
        _selectedHovergradient->setColorAt(1, QColor(0x2F, 0x2F, 0x2F));
    }
}

void ElaListViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    QRect itemRect = option.rect;
    itemRect.setTop(itemRect.top() + 2);
    itemRect.setBottom(itemRect.bottom() - 2);
    QPainterPath path;
    path.addRoundedRect(itemRect, 8, 8);
    if (option.showDecorationSelected && (option.state & QStyle::State_Selected))
    {
        if (option.state & QStyle::State_MouseOver)
        {
            // 选中时覆盖
            painter->fillPath(path, *_selectedHovergradient);
        }
        else
        {
            // 选中
            painter->fillPath(path, *_selectedgradient);
        }
    }
    else
    {
        if (option.state & QStyle::State_MouseOver)
        {
            // 覆盖时颜色
            painter->fillPath(path, *_hovergradient);
        }
    }
    //文字绘制
    QString text = index.data(Qt::DisplayRole).toString();
    painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
    painter->drawText(itemRect, Qt::AlignVCenter | Qt::AlignLeft, text);
    painter->restore();
}
