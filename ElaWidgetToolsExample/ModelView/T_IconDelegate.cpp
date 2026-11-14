#include "T_IconDelegate.h"

#include "ElaTheme.h"
#include "ElaWidgetToolsDef.h"

#include <QPainter>

T_IconDelegate::T_IconDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

T_IconDelegate::~T_IconDelegate()
{
}

void T_IconDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, viewOption, index);
    QStringList iconList = index.data(Qt::UserRole).toStringList();
    if (iconList.count() != 2)
    {
        return;
    }
    QString iconName = iconList.at(0);
    QString iconValue = iconList.at(1);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QFont iconFont("ElaAwesome");
    iconFont.setPixelSize(28);
    painter->setFont(iconFont);
    painter->setPen(ElaThemeColor(_themeMode, BasicText));
    painter->drawText(QRect(option.rect.x(), option.rect.y() + 12, option.rect.width(), 36), Qt::AlignCenter, iconValue);
    QFont labelFont = option.font;
    labelFont.setPixelSize(12);
    painter->setFont(labelFont);
    const QRect labelRect(option.rect.x() + 6, option.rect.y() + 58, option.rect.width() - 12, 20);
    painter->drawText(labelRect, Qt::AlignCenter, painter->fontMetrics().elidedText(iconName, Qt::ElideRight, labelRect.width()));
    painter->restore();
}

QSize T_IconDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(104, 88);
}
