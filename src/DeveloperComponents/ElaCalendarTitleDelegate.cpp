#include "ElaCalendarTitleDelegate.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaTheme.h"
ElaCalendarTitleDelegate::ElaCalendarTitleDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaCalendarTitleDelegate::~ElaCalendarTitleDelegate()
{
}

void ElaCalendarTitleDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QRectF itemRect = option.rect;
    // 文字绘制
    QString title = index.data(Qt::UserRole).toString();
    if (!title.isEmpty())
    {
        painter->setPen(ElaThemeColor(_themeMode, BasicText));
        QFont font = painter->font();
        font.setWeight(QFont::Bold);
        painter->setFont(font);
        painter->drawText(itemRect, Qt::AlignCenter, title);
    }
    painter->restore();
    QStyledItemDelegate::paint(painter, option, index);
}

QSize ElaCalendarTitleDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(42, 30);
}
