#include "ElaBreadcrumbBarDelegate.h"

#include <QPainter>

#include "ElaApplication.h"
ElaBreadcrumbBarDelegate::ElaBreadcrumbBarDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = ElaApplication::getInstance()->getThemeMode();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) {
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
    painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
    QRect itemRect = option.rect;
    QString breadcrumbDisplayData = index.data(Qt::DisplayRole).toString();
    QString breadcrumbUserData = index.data(Qt::UserRole).toString();
    if (breadcrumbUserData != "LastBreadcrumb")
    {
        if (option.state & QStyle::State_HasFocus)
        {
            //鼠标按下
            painter->setPen(QColor(0x86, 0x86, 0x8A));
        }
        else
        {
            //不为最后一个 且没有被鼠标覆盖
            if (!(option.state & QStyle::State_MouseOver))
            {
                painter->setPen(_themeMode == ElaApplicationType::Light ? QColor(0x5B, 0x5B, 0x5E) : QColor(0xDE, 0xDE, 0xDE));
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
