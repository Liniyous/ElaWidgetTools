#include "ElaCalendarDelegate.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaTheme.h"
ElaCalendarDelegate::ElaCalendarDelegate(ElaCalendarModel* calendarModel, QObject* parent)
    : QStyledItemDelegate{parent}
{
    _pIsTransparent = false;
    _calendarModel = calendarModel;
    _pItemWidth = 42;
    _pItemHeight = 42;
    _nowDate = QDate::currentDate();
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
    connect(_calendarModel, &ElaCalendarModel::displayModeChanged, this, &ElaCalendarDelegate::onCalendarModelDisplayModeChanged);
}

ElaCalendarDelegate::~ElaCalendarDelegate()
{
}

void ElaCalendarDelegate::onCalendarModelDisplayModeChanged()
{
    switch (_calendarModel->getDisplayMode())
    {
    case YearMode:
    case MonthMode:
    {
        _pItemWidth = 53;
        _pItemHeight = 60;
        break;
    }
    case DayMode:
    {
        _pItemWidth = 42;
        _pItemHeight = 42;
        break;
    }
    }
}

void ElaCalendarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (_pIsTransparent)
    {
        return;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    ElaCalendarType displayModel = _calendarModel->getDisplayMode();
    switch (displayModel)
    {
    case YearMode:
    case MonthMode:
    {
        _drawYearOrMonth(painter, option, index);
        break;
    }
    case DayMode:
    {
        _drawDays(painter, option, index);
        break;
    }
    }
    painter->restore();
    QStyledItemDelegate::paint(painter, option, index);
}

QSize ElaCalendarDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(_pItemWidth, _pItemHeight);
}

void ElaCalendarDelegate::_drawYearOrMonth(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QRectF itemRect = option.rect;
    bool isNow = false;
    ElaCalendarData data = index.data(Qt::UserRole).value<ElaCalendarData>();
    qreal baseRadius = _pItemWidth * 0.5;
    // 当前日期绘制
    ElaCalendarType displayMode = _calendarModel->getDisplayMode();
    if ((displayMode == ElaCalendarType::YearMode && data.year == _nowDate.year()) || (displayMode == ElaCalendarType::MonthMode && data.month == _nowDate.month() && data.year == _nowDate.year()))
    {
        isNow = true;
        QColor drawColor = option.state.testFlag(QStyle::State_MouseOver) ? ElaThemeColor(_themeMode, PrimaryHover) : ElaThemeColor(_themeMode, PrimaryNormal);
        painter->setPen(Qt::NoPen);
        painter->setBrush(drawColor);
        painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
    }
    else
    {
        // 覆盖和选中效果绘制
        if (option.state.testFlag(QStyle::State_MouseOver))
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(ElaThemeColor(_themeMode, BasicHover));
            painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
        }
    }

    // 文字绘制
    painter->setPen(isNow ? ElaThemeColor(_themeMode, BasicTextInvert) : ElaThemeColor(_themeMode, BasicText));
    painter->drawText(itemRect, Qt::AlignCenter, displayMode == ElaCalendarType::YearMode ? QString::number(data.year) : QString::number(data.month));
    QString desText = data.desText;
    if (!desText.isEmpty())
    {
        itemRect.adjust(0, 10, 0, 0);
        QFont font = painter->font();
        font.setPixelSize(9);
        painter->setFont(font);
        painter->drawText(itemRect, Qt::AlignTop | Qt::AlignHCenter, desText);
    }
}

void ElaCalendarDelegate::_drawDays(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    qreal penWidth = 1.5;
    qreal baseRadius = _pItemWidth * 0.5 - penWidth;
    QVariant variant = index.data(Qt::UserRole);
    if (variant.isValid())
    {
        QRectF itemRect = option.rect;
        bool isNow = false;
        ElaCalendarData data = variant.value<ElaCalendarData>();
        // 当前日期绘制
        if (data.year == _nowDate.year() && data.month == _nowDate.month() && data.day == _nowDate.day())
        {
            isNow = true;
            QColor drawColor = option.state.testFlag(QStyle::State_MouseOver) ? ElaThemeColor(_themeMode, PrimaryHover) : ElaThemeColor(_themeMode, PrimaryNormal);
            if (option.state.testFlag(QStyle::State_Selected))
            {
                painter->setPen(QPen(drawColor, penWidth));
                painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
                painter->setBrush(drawColor);
                painter->setPen(Qt::NoPen);
                painter->drawEllipse(itemRect.center(), baseRadius - 2 * penWidth, baseRadius - 2 * penWidth);
            }
            else
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(drawColor);
                painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
            }
        }
        else
        {
            // 覆盖和选中效果绘制
            if (option.state.testFlag(QStyle::State_Selected))
            {
                painter->setPen(QPen(option.state.testFlag(QStyle::State_MouseOver) ? ElaThemeColor(_themeMode, PrimaryHover) : ElaThemeColor(_themeMode, PrimaryNormal), penWidth));
                painter->setBrush(Qt::NoBrush);
                painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
            }
            if (option.state.testFlag(QStyle::State_MouseOver))
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(ElaThemeColor(_themeMode, BasicHover));
                if (option.state.testFlag(QStyle::State_Selected))
                {
                    painter->drawEllipse(itemRect.center(), baseRadius - penWidth, baseRadius - penWidth);
                }
                else
                {
                    painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
                }
            }
        }

        // 文字绘制
        painter->setPen(isNow ? ElaThemeColor(_themeMode, BasicTextInvert) : ElaThemeColor(_themeMode, BasicText));
        painter->drawText(itemRect, Qt::AlignCenter, QString::number(data.day));
        QString desText = data.desText;
        if (!desText.isEmpty())
        {
            itemRect.adjust(0, 2 * penWidth, 0, 0);
            QFont font = painter->font();
            font.setPixelSize(9);
            painter->setFont(font);
            painter->drawText(itemRect, Qt::AlignTop | Qt::AlignHCenter, desText);
        }
    }
}
