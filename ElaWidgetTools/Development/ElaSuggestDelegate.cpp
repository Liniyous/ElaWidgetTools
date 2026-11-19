#include "ElaSuggestDelegate.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaControlStyle.h"
#include "ElaSuggestBox.h"
#include "ElaSuggestBoxPrivate.h"
#include "ElaSuggestModel.h"
#include "ElaTheme.h"

#include <QPainter>
#include <QPainterPath>

ElaSuggestDelegate::ElaSuggestDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaSuggestDelegate::~ElaSuggestDelegate()
{
}

void ElaSuggestDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    int margin = 2;
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    ElaSuggestModel* model = dynamic_cast<ElaSuggestModel*>(const_cast<QAbstractItemModel*>(index.model()));
    ElaSuggestion* suggest = model->getSearchSuggestion(index.row());
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    QRect optionRect = option.rect;
    optionRect.adjust(margin * 2, margin, -margin * 2, -margin);
    path.addRoundedRect(optionRect, 8, 8);
    const QColor background = ElaControlStyle::getItemBackgroundColor(_themeMode, option.state);
    painter->fillPath(path, ElaColorAnimation::getColor(option.widget, background, "ItemBackground", index));
    //文字绘制
    painter->setPen(ElaThemeColor(_themeMode, BasicText));
    painter->drawText(option.rect.adjusted(37, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, suggest->getSuggestText());

    //图标绘制
    if (suggest->getElaIcon() != ElaIconType::None)
    {
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(eApp->getFontPixelSize() + 4);
        painter->setFont(iconFont);
        painter->drawText(option.rect.adjusted(11, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, QChar(suggest->getElaIcon()));
    }
    painter->restore();
}

QSize ElaSuggestDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(ElaWidgetToolsSize::DefaultListItemHeight);
    return size;
}
