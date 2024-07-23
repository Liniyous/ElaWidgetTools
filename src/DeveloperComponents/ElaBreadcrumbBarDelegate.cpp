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
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    painter->setPen(_themeMode == ElaThemeType::Light ? Qt::black : Qt::white);
    QRect itemRect = option.rect;
    QString breadcrumbDisplayData = index.data(Qt::DisplayRole).toString();
    QString breadcrumbUserData = index.data(Qt::UserRole).toString();
    if (breadcrumbUserData != "LastBreadcrumb")
    {
        if (option.state & QStyle::State_HasFocus)
        {
            //鼠标按下
            painter->setPen(ElaThemeColor(_themeMode, BreadcrumbBarTextHasFocus));
        }
        else
        {
            //不为最后一个 且没有被鼠标覆盖
            if (!(option.state & QStyle::State_MouseOver))
            {
                painter->setPen(ElaThemeColor(_themeMode, BreadcrumbBarText));
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
        iconFont.setPixelSize(22);
        painter->setFont(iconFont);
        itemRect.setX(itemRect.x() - itemRect.width() * 0.36);
        painter->drawText(itemRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::AngleRight));
    }
    painter->restore();
}
