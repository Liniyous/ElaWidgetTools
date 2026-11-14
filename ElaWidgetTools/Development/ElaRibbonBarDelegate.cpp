#include "ElaRibbonBarDelegate.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaRibbonBarModel.h"
#include "ElaTheme.h"

#include <QDebug>
#include <QMenu>
#include <QPainter>
#include <QStyleOptionViewItem>

ElaRibbonBarDelegate::ElaRibbonBarDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _pBottomMargin = 13;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaRibbonBarDelegate::~ElaRibbonBarDelegate()
{
}

void ElaRibbonBarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    int fontPixelSize = eApp->getFontPixelSize();
    const auto ribbonAction = index.data(Qt::UserRole).value<QAction*>();
    if (!ribbonAction)
    {
        return;
    }
    if (ribbonAction->property("ElaRibbonWidget").value<QWidget*>())
    {
        return;
    }
    if (!ribbonAction->isVisible())
    {
        return;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    QRect itemRect = option.rect;
    QString groupName = ribbonAction->property("ElaRibbonGroupName").toString();
    if (!groupName.isEmpty())
    {
        // 保留 pBottomMargin px用于组别绘制
        itemRect.adjust(0, 0, 0, -_pBottomMargin);
    }
    // 背景绘制
    painter->setPen(Qt::NoPen);
    bool isViewEnable = option.state.testFlag(QStyle::State_Enabled);
    bool isActionEnable = ribbonAction->isEnabled();
    bool isEnable = isViewEnable & isActionEnable;
    const QColor background = !isEnable                                               ? QColor(Qt::transparent)
        : index == _pPressIndex                                                       ? ElaThemeColor(_themeMode, BasicPressAlpha)
        : option.state.testFlag(QStyle::State_MouseOver) || ribbonAction->isChecked() ? ElaThemeColor(_themeMode, BasicHoverAlpha)
                                                                                      : QColor(Qt::transparent);
    painter->setBrush(ElaColorAnimation::getColor(option.widget, background, "ItemBackground", index));
    painter->drawRoundedRect(itemRect, 5, 5);
    QFont textFont = painter->font();
    textFont.setPixelSize(fontPixelSize - 1);
    painter->setFont(textFont);
    // 绘制Action
    if (!ribbonAction->icon().isNull())
    {
        // 绘制QIcon
        QIcon icon = ribbonAction->icon();
        int pixWidth = itemRect.width() * 0.4;
        painter->drawPixmap(QRect(itemRect.x() + (itemRect.width() - pixWidth) / 2, itemRect.y() + 10, pixWidth, pixWidth), icon.pixmap(pixWidth, pixWidth, option.state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled, option.state & QStyle::State_Selected ? QIcon::On : QIcon::Off));
        // 绘制文字
        painter->setPen(isEnable ? ElaThemeColor(_themeMode, BasicText) : ElaThemeColor(_themeMode, BasicTextDisable));
        itemRect.adjust(0, 0, 0, -_pBottomMargin / 2);
        painter->drawText(itemRect, Qt::AlignHCenter | Qt::AlignBottom | Qt::TextWordWrap | Qt::TextWrapAnywhere, ribbonAction->text());
    }
    else
    {
        painter->setPen(isEnable ? ElaThemeColor(_themeMode, BasicText) : ElaThemeColor(_themeMode, BasicTextDisable));
        auto elaIcon = ribbonAction->property("ElaRibbonIcon").toString();
        if (elaIcon.isEmpty())
        {
            // 绘制文字
            painter->drawText(itemRect, Qt::AlignCenter | Qt::TextWordWrap | Qt::TextWrapAnywhere, ribbonAction->text());
        }
        else
        {
            // 绘制图标
            painter->save();
            if (isEnable && ribbonAction->isChecked())
            {
                painter->setPen(ElaThemeColor(_themeMode, PrimaryNormal));
            }
            QFont iconFont = QFont("ElaAwesome");
            if (groupName.isEmpty())
            {
                iconFont.setPixelSize(fontPixelSize + 15);
                painter->setFont(iconFont);
                painter->drawText(QRect(itemRect.x(), itemRect.y() + 10, itemRect.width(), itemRect.height()), Qt::AlignHCenter | Qt::AlignTop, elaIcon);
            }
            else
            {
                iconFont.setPixelSize(fontPixelSize + 9);
                painter->setFont(iconFont);
                painter->drawText(QRect(itemRect.x(), itemRect.y() + 10, itemRect.width(), itemRect.height()), Qt::AlignHCenter | Qt::AlignTop, elaIcon);
            }
            painter->restore();
            // 绘制文字
            itemRect.adjust(0, 0, 0, -_pBottomMargin / 2);
            painter->drawText(itemRect, Qt::AlignHCenter | Qt::AlignBottom | Qt::TextWordWrap | Qt::TextWrapAnywhere, ribbonAction->text());
        }
        // 绘制下拉图标
        if (ribbonAction->menu())
        {
            painter->setPen(ElaThemeColor(_themeMode, BasicText));
            painter->save();
            QFont iconFont = QFont("ElaAwesome");
            iconFont.setPixelSize(fontPixelSize + 4);
            painter->setFont(iconFont);
            painter->drawText(itemRect.adjusted(0, 0, -6, 0), Qt::AlignVCenter | Qt::AlignRight, QChar(ElaIconType::AngleDown));
            painter->restore();
        }
    }
    painter->restore();
}

QSize ElaRibbonBarDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const auto ribbonAction = index.data(Qt::UserRole).value<QAction*>();
    if (!ribbonAction)
    {
        return {};
    }
    const auto indexWidget = ribbonAction->property("ElaRibbonWidget").value<QWidget*>();
    if (indexWidget)
    {
        return indexWidget->size();
    }
    if (!ribbonAction->isVisible())
    {
        return {};
    }
    auto size = QStyledItemDelegate::sizeHint(option, index);
    if (size.width() < 70)
    {
        size.setWidth(70);
    }
    return size;
}
