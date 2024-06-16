#include "ElaComboBoxDelegate.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaApplication.h"
ElaComboBoxDelegate::ElaComboBoxDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = ElaApplication::getInstance()->getThemeMode();
    _hovergradient = new QLinearGradient(0, 0, 290, 38);
    _hovergradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
    _hovergradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
    _selectedgradient = new QLinearGradient(0, 0, 290, 38);
    _selectedgradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
    _selectedgradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
    _selectedHovergradient = new QLinearGradient(0, 0, 290, 38);
    _selectedHovergradient->setColorAt(0, QColor(0xEC, 0xEC, 0xF3));
    _selectedHovergradient->setColorAt(1, QColor(0xED, 0xEC, 0xF3));
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, &ElaComboBoxDelegate::onThemeChanged);
}

ElaComboBoxDelegate::~ElaComboBoxDelegate()
{
}

void ElaComboBoxDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    int margin = 2;
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    QRect optionRect = option.rect;
    optionRect.setTopLeft(QPoint(optionRect.left() + margin, optionRect.top() + margin));
    optionRect.setBottomRight(QPoint(optionRect.right() - margin, optionRect.bottom() - margin));
    path.addRoundedRect(optionRect, 5, 5);

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
        //选中Mark
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0x0E, 0x6F, 0xC3));
        painter->drawRoundedRect(QRectF(optionRect.x() + 3, optionRect.y() + optionRect.height() * 0.2, 3, optionRect.height() - +optionRect.height() * 0.4), 2, 2);
    }
    else
    {
        if (option.state & QStyle::State_MouseOver)
        {
            // 覆盖时颜色
            painter->fillPath(path, *_hovergradient);
        }
    }

    // 文字绘制
    painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
    painter->drawText(QRect(option.rect.x() + 15, option.rect.y(), option.rect.width() - 15, option.rect.height()), Qt::AlignVCenter, index.data().toString());
    painter->restore();
}

void ElaComboBoxDelegate::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    _themeMode = themeMode;
    if (themeMode == ElaApplicationType::Light)
    {
        _hovergradient = new QLinearGradient(0, 0, 290, 38);
        _hovergradient->setColorAt(0, QColor(0xEC, 0xEC, 0xEC));
        _hovergradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
        _selectedgradient = new QLinearGradient(0, 0, 290, 38);
        _selectedgradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
        _selectedgradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
        _selectedHovergradient = new QLinearGradient(0, 0, 290, 38);
        _selectedHovergradient->setColorAt(0, QColor(0xEC, 0xEC, 0xF3));
        _selectedHovergradient->setColorAt(1, QColor(0xED, 0xEC, 0xF3));
    }
    else
    {
        _hovergradient = new QLinearGradient(0, 0, 290, 38);
        _hovergradient->setColorAt(0, QColor(0x38, 0x38, 0x38));
        _hovergradient->setColorAt(1, QColor(0x38, 0x38, 0x38));
        _selectedgradient = new QLinearGradient(0, 0, 290, 38);
        _selectedgradient->setColorAt(0, QColor(0x42, 0x42, 0x42));
        _selectedgradient->setColorAt(1, QColor(0x42, 0x42, 0x42));
        _selectedHovergradient = new QLinearGradient(0, 0, 290, 38);
        _selectedHovergradient->setColorAt(0, QColor(0x4A, 0x4A, 0x4A));
        _selectedHovergradient->setColorAt(1, QColor(0x4A, 0x4A, 0x4A));
    }
}
