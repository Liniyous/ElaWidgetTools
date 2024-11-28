#include "ElaBreadcrumbBarDelegate.h"

#include <QPainter>

#include "ElaTheme.h"
ElaBreadcrumbBarDelegate::ElaBreadcrumbBarDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaBreadcrumbBarDelegate::~ElaBreadcrumbBarDelegate()
{
}

void ElaBreadcrumbBarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(ElaThemeColor(_themeMode, BasicText));
    QRect itemRect = option.rect;
    QString breadcrumbDisplayData = index.data(Qt::DisplayRole).toString();
    QString breadcrumbUserData = index.data(Qt::UserRole).toString();
    if (breadcrumbUserData != "LastBreadcrumb")
    {
        if (_pPressIndex == index)
        {
            //鼠标按下
            painter->setPen(ElaThemeColor(_themeMode, BasicTextPress));
        }
        else
        {
            //不为最后一个 且没有被鼠标覆盖
            if (!(option.state & QStyle::State_MouseOver))
            {
                painter->setPen(ElaThemeColor(_themeMode, BasicTextNoFocus));
            }
        }
    }
    if (breadcrumbDisplayData != ">")
    {
        painter->drawText(itemRect, Qt::AlignVCenter | Qt::AlignLeft, breadcrumbDisplayData);
    }
    else
    {
        //分隔符
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(painter->font().pixelSize() * 0.785);
        painter->setFont(iconFont);
        itemRect.setX(itemRect.x() - itemRect.width() * 0.36);
        painter->drawText(itemRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::AngleRight));
    }
    painter->restore();
}
