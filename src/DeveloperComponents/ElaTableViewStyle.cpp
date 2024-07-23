#include "ElaTableViewStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionViewItem>

#include "ElaTheme.h"
ElaTableViewStyle::ElaTableViewStyle(QStyle* style)
{
    _hovergradient = new QLinearGradient(0, 0, 290, 38);
    _hovergradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
    _hovergradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
    _selectedgradient = new QLinearGradient(0, 0, 290, 38);
    _selectedgradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
    _selectedgradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
    _selectedHovergradient = new QLinearGradient(0, 0, 290, 38);
    _selectedHovergradient->setColorAt(0, QColor(0xEC, 0xEC, 0xF3));
    _selectedHovergradient->setColorAt(1, QColor(0xED, 0xEC, 0xF3));
    connect(eTheme, &ElaTheme::themeModeChanged, this, &ElaTableViewStyle::onThemeChanged);
}

ElaTableViewStyle::~ElaTableViewStyle()
{
}

void ElaTableViewStyle::drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const
{
    switch (pe)
    {
    case QStyle::PE_PanelItemViewRow:
    {
        drawPanelItemViewRow(opt, p, w);
        break;
    }
    default:
    {
        QProxyStyle::drawPrimitive(pe, opt, p, w);
        break;
    }
    }
}

void ElaTableViewStyle::drawPanelItemViewRow(const QStyleOption* opt, QPainter* p, const QWidget* w) const
{
    p->save();
    if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt))
    {
        p->setRenderHint(QPainter::Antialiasing);
        p->setPen(Qt::NoPen);
        QRect rect = opt->rect;
        if (vopt->showDecorationSelected && (vopt->state & QStyle::State_Selected))
        {
            if (vopt->state & QStyle::State_MouseOver)
            {
                // 选中时覆盖
                p->setBrush(*_selectedHovergradient);
                p->drawRoundedRect(rect, 9, 9);
            }
            else
            {
                // 选中
                p->setBrush(*_selectedgradient);
                p->drawRoundedRect(rect, 9, 9);
            }
        }
        else
        {
            if (vopt->state & QStyle::State_MouseOver)
            {
                // 覆盖时颜色
                p->setBrush(*_hovergradient);
                p->drawRoundedRect(rect, 9, 9);
            }
        }
    }
    p->restore();
}

void ElaTableViewStyle::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    if (themeMode == ElaThemeType::Light)
    {
        _hovergradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
        _hovergradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
        _selectedgradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
        _selectedgradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
        _selectedHovergradient->setColorAt(0, QColor(0xEC, 0xEC, 0xF3));
        _selectedHovergradient->setColorAt(1, QColor(0xED, 0xEC, 0xF3));
    }
    else
    {
        _hovergradient->setColorAt(0, QColor(0x27, 0x27, 0x27));
        _hovergradient->setColorAt(1, QColor(0x27, 0x27, 0x27));
        _selectedgradient->setColorAt(0, QColor(0x35, 0x35, 0x35));
        _selectedgradient->setColorAt(1, QColor(0x35, 0x35, 0x35));
        _selectedHovergradient->setColorAt(0, QColor(0x2F, 0x2F, 0x2F));
        _selectedHovergradient->setColorAt(1, QColor(0x2F, 0x2F, 0x2F));
    }
}
