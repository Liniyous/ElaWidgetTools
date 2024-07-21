#include "ElaComboBoxStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaApplication.h"
ElaComboBoxStyle::ElaComboBoxStyle(QStyle* style)
{
    _pExpandIconRotate = 0;
    _pExpandMarkWidth = 0;
    _themeMode = eApp->getThemeMode();
    connect(eApp, &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaComboBoxStyle::~ElaComboBoxStyle()
{
}

void ElaComboBoxStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_Widget:
    {
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void ElaComboBoxStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ComboBoxLabel:
    {
        return;
    }
    case QStyle::CE_ShapedFrame:
    {
        //container区域
        if (widget->objectName() == "ElaComboBoxContainer")
        {
            QRect viewRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            eApp->drawEffectShadow(painter, viewRect, _shadowBorderWidth, 6);
            QRect foregroundRect(viewRect.x() + _shadowBorderWidth, viewRect.y(), viewRect.width() - 2 * _shadowBorderWidth, viewRect.height() - _shadowBorderWidth);
            painter->setPen(_themeMode == ElaApplicationType::Light ? QColor(0xBE, 0xBA, 0xBE) : QColor(0x52, 0x50, 0x52));
            painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xF9, 0xF9, 0xF9) : QColor(0x2D, 0x2D, 0x2D));
            painter->drawRoundedRect(foregroundRect, 3, 3);
            painter->restore();
        }
        return;
    }
    case QStyle::CE_ItemViewItem:
    {
        //覆盖高亮
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            int margin = 2;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            painter->setPen(Qt::NoPen);
            QPainterPath path;
            QRect optionRect = option->rect;
            optionRect.adjust(margin, margin, -margin, -margin);
            path.addRoundedRect(optionRect, 5, 5);
            if (option->state & QStyle::State_Selected)
            {
                if (option->state & QStyle::State_MouseOver)
                {
                    // 选中时覆盖
                    painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xEA, 0xE9, 0xF0) : QColor(0x33, 0x33, 0x33));
                    painter->drawPath(path);
                }
                else
                {
                    // 选中
                    painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xE5, 0xE4, 0xEB) : QColor(0x40, 0x40, 0x40));
                    painter->drawPath(path);
                }
                //选中Mark
                painter->setPen(Qt::NoPen);
                painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0x0E, 0x6F, 0xC3) : QColor(0x4C, 0xA0, 0xE0));
                painter->drawRoundedRect(QRectF(optionRect.x() + 3, optionRect.y() + optionRect.height() * 0.2, 3, optionRect.height() - +optionRect.height() * 0.4), 2, 2);
            }
            else
            {
                if (option->state & QStyle::State_MouseOver)
                {
                    // 覆盖时颜色
                    painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xE5, 0xE4, 0xEB) : QColor(0x40, 0x40, 0x40));
                    painter->drawPath(path);
                }
            }
            // 文字绘制
            painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
            painter->drawText(QRect(option->rect.x() + 15, option->rect.y(), option->rect.width() - 15, option->rect.height()), Qt::AlignVCenter, vopt->text);
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

void ElaComboBoxStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
    case QStyle::CC_ComboBox:
    {
        //主体显示绘制
        if (const QStyleOptionComboBox* copt = qstyleoption_cast<const QStyleOptionComboBox*>(option))
        {
            painter->save();
            painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
            //背景绘制
            if (_themeMode == ElaApplicationType::Light)
            {
                painter->setPen(QPen(QColor(0xDF, 0xDF, 0xDF), 1));
                painter->setBrush(copt->state.testFlag(QStyle::State_MouseOver) ? QColor(0xF6, 0xF6, 0xF6) : QColor(0xFD, 0xFD, 0xFD));
            }
            else
            {
                painter->setPen(QPen(QColor(0x4B, 0x4B, 0x4D), 1));
                painter->setBrush(copt->state.testFlag(QStyle::State_MouseOver) ? QColor(0x44, 0x44, 0x44) : QColor(0x3B, 0x3B, 0x3B));
            }
            QRect comboBoxRect = copt->rect;
            comboBoxRect.adjust(_shadowBorderWidth, 1, -_shadowBorderWidth, -1);
            painter->drawRoundedRect(comboBoxRect, 3, 3);
            //文字绘制
            QRect textRect = subControlRect(QStyle::CC_ComboBox, copt, QStyle::SC_ScrollBarSubLine, widget);
            painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
            painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, copt->currentText);
            //展开指示器绘制
            painter->setPen(Qt::NoPen);
            painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0x0E, 0x6F, 0xC3) : QColor(0x4C, 0xA0, 0xE0));
            painter->drawRoundedRect(QRectF(comboBoxRect.center().x() - _pExpandMarkWidth, comboBoxRect.height() - 3, _pExpandMarkWidth * 2, 3), 2, 2);
            // 展开图标绘制
            QRect expandIconRect = subControlRect(QStyle::CC_ComboBox, copt, QStyle::SC_ScrollBarAddPage, widget);
            if (expandIconRect.isValid())
            {
                QFont iconFont = QFont("ElaAwesome");
                iconFont.setPixelSize(17);
                painter->setFont(iconFont);
                painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
                painter->translate(expandIconRect.x() + (qreal)expandIconRect.width() / 2, expandIconRect.y() + (qreal)expandIconRect.height() / 2);
                painter->rotate(_pExpandIconRotate);
                painter->translate(-expandIconRect.x() - (qreal)expandIconRect.width() / 2, -expandIconRect.y() - (qreal)expandIconRect.height() / 2);
                painter->drawText(expandIconRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::AngleDown));
                painter->restore();
            }
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QRect ElaComboBoxStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const
{
    switch (cc)
    {
    case QStyle::CC_ComboBox:
    {
        switch (sc)
        {
        case QStyle::SC_ScrollBarSubLine:
        {
            //文字区域
            QRect textRect = QProxyStyle::subControlRect(cc, opt, sc, widget);
            textRect.setLeft(16);
            textRect.setRight(textRect.right() - 15);
            return textRect;
        }
        case QStyle::SC_ScrollBarAddPage:
        {
            //展开图标区域
            QRect expandIconRect = QProxyStyle::subControlRect(cc, opt, sc, widget);
            expandIconRect.setLeft(expandIconRect.left() - 25);
            return expandIconRect;
        }
        default:
        {
            break;
        }
        }
        break;
    }
    default:
    {
        break;
    }
    }

    return QProxyStyle::subControlRect(cc, opt, sc, widget);
}

QSize ElaComboBoxStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case QStyle::CT_ItemViewItem:
    {
        QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        itemSize.setHeight(35);
        return itemSize;
    }
    case QStyle::CT_ComboBox:
    {
        QSize comboBoxSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        comboBoxSize.setWidth(comboBoxSize.width() + 26);
        return comboBoxSize;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}
